#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <json.h>
#include <string.h>
#include <unistd.h>

#ifdef FREEBSD_OS
#include <netinet/if_ether.h>
#elif LINUX_OS
#include <netinet/ether.h>
#endif

#include "logging.h"
#include "ethertype.h"
#include "timeutils.h"
#include "packet_chain.h"

#include "processor.h"

/**
 * @todo This function should have nested json structuring.
 * @todo Create unit tests to accurately check nano second timestamping. This should be
 *          an intergrated test which connect to Elasticsearch and do the testing.
 * @todo Most of the time protocol is decided by the port number. Sometimes
 *          protocol is decided by reading the IP packet data. And sometimes
 *          other techniques aso used. So we need to specifically output which
 *          technique is used to identify the protocol.
*/
static void print_packet_info(packet_link_t *pkt_link)
{
    char src_mac[18], dst_mac[18];
    struct json_object *jobj = json_object_new_object();

    const uint8_t *packet = pkt_link->packet;
    struct pcap_pkthdr *packet_header = pkt_link->packet_header;

    /* Timestamping */
    struct timespec ts;
    char timestr[32];
    ts.tv_sec = (double)packet_header->ts.tv_sec;
    ts.tv_nsec = (double)packet_header->ts.tv_usec;

    if (timespec_iso8601(timestr, sizeof(timestr), &ts) != 0) {
        log_error("timespec_iso8601 function failure failed!");
        strcpy(timestr, "timestamping error");
    }

    /* Get Ethernet header related data */
    struct ether_header *eth_header = (struct ether_header *)packet;
    ether_ntoa_r((struct ether_addr *)eth_header->ether_shost, src_mac);
    ether_ntoa_r((struct ether_addr *)eth_header->ether_dhost, dst_mac);
    fl_eth_type_t *eth_type = get_fl_eth_type(ntohs(eth_header->ether_type));

    json_object_object_add(jobj, "timestamp_", json_object_new_string(timestr));
    json_object_object_add(jobj, "packet_header.caplen", json_object_new_int(packet_header->caplen));
    json_object_object_add(jobj, "packet_header.len", json_object_new_int(packet_header->len));
    json_object_object_add(jobj, "ethernet.type.code", json_object_new_string(eth_type->short_code));
    json_object_object_add(jobj, "ethernet.type.value", json_object_new_int64(ntohs(eth_header->ether_type)));
    json_object_object_add(jobj, "ethernet.type.desc", json_object_new_string(eth_type->type_desc));
    json_object_object_add(jobj, "ethernet.addr.dst", json_object_new_string(dst_mac));
    json_object_object_add(jobj, "ethernet.addr.src", json_object_new_string(src_mac));
    json_object_object_add(jobj, "log.level", json_object_new_string("INFO"));

    printf("%s\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));

    json_object_put(jobj);
}


static void packet_chain_handler(unsigned char *user,
                                  const struct pcap_pkthdr *packet_header,
                                  const uint8_t *packet_body)
{
    packet_chain_t *pkt_chain = (packet_chain_t *)user;

    packet_chain_put(packet_header, packet_body, pkt_chain);
}

/**
 * @brief This function triggers the printing of packet details as json log
 * 
 * This function runs as a thread and it will be triggered by
 *  @ref processor_packet_stream
*/
static void *packet_printer_thread(void *arg)
{
    packet_chain_t *chain = (packet_chain_t *)arg;
    packet_link_t *link = NULL;

    while (1) {
        sleep(1);

        log_info("Starting to free memory");

        while ((link = packet_chain_pop(chain)) != NULL) {
            print_packet_info(link);
            free_packet_link(link);
        }
    }

    return NULL;
}

/**
 * @todo Need to apply a way to handle packets sent from stdin.
 *        @ref STREAM_STDIN.
*/
int processor_packet_stream(char *source, packet_chain_t *chain,
                            processor_opt_t p_opt)
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    pthread_t thrd1;

    if (p_opt & STREAM_NET_INTERFACE) {
        handle = pcap_create(source, error_buffer);
        if (handle == NULL) {
            fprintf(stderr, "Error creating packet capture handle: %s\n", error_buffer);
            return 1;
        }

        if (pcap_set_promisc(handle, 1) != 0) {
            fprintf(stderr, "Error setting promiscuous mode\n");
            pcap_close(handle);
            return 1;
        }

        if (pcap_set_snaplen(handle, 65535) != 0) {
            fprintf(stderr, "Error setting snap length\n");
            pcap_close(handle);
            return 1;
        }

        if (pcap_set_timeout(handle, 10) != 0) {
            fprintf(stderr, "Error setting capture timeout\n");
            pcap_close(handle);
            return 1;
        }

        /**
         * @brief Nano second percision activation code.
         * 
         * If this code is activated, please make sure to reconfigure time
         * calculation at function \ref print_packet_info
        */
        if (pcap_set_tstamp_precision(handle, PCAP_TSTAMP_PRECISION_NANO) != 0) {
            log_error("Error activating Nanosecond percision");
            pcap_close(handle);
            return 1;
        }

        if (pcap_activate(handle) != 0) {
            fprintf(stderr, "Error activating packet capture handle: %s\n", pcap_geterr(handle));
            pcap_close(handle);
            return 1;
        }
    } else if (p_opt & STREAM_FILE) {
        // handle = pcap_open_offline(source, error_buffer);
        handle = pcap_open_offline_with_tstamp_precision(source,
                                PCAP_TSTAMP_PRECISION_NANO,
                                error_buffer);
        if (handle == NULL) {
            printf("Error opening file: %s\n", error_buffer);
            return 1;
        }
    }

    switch (pcap_get_tstamp_precision(handle)) {
        case PCAP_TSTAMP_PRECISION_MICRO:
            log_info("Timestamp percision: PCAP_TSTAMP_PRECISION_MICRO");
            break;
        case PCAP_TSTAMP_PRECISION_NANO:
            log_info("Timestamp percision: PCAP_TSTAMP_PRECISION_NANO");
            break;
        default:
            log_error("SOme other value");
    }

    /* Currently this program supports only Ethernet card types.
     * New formats will be included in the future if possible.
     */
    if (pcap_datalink(handle) != DLT_EN10MB) {
        log_error("This program only supports Ethernet cards!");
        pcap_close(handle);
        return 1;
    }

    if (p_opt & JSON_LOGGER)
        pthread_create(&thrd1, NULL, packet_printer_thread, chain);

    pcap_loop(handle, 0, packet_chain_handler, (unsigned char *)chain);
    pcap_close(handle);

    if (p_opt & JSON_LOGGER)
        pthread_join(thrd1, NULL);

    return 0;
}
