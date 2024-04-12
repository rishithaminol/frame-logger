#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <json.h>
#include <string.h>

#include "logging.h"
#include "ethertype.h"


/**
 * @todo This function should have nested json structuring.
*/
static void print_packet_info(const uint8_t *packet, struct pcap_pkthdr packet_header)
{
    double t_sec = (double)packet_header.ts.tv_sec;
    double t_usec = (double)packet_header.ts.tv_usec;
    struct json_object *jobj = json_object_new_object();

    /* Determine Ethernet type */
    struct ether_header *eth_header = (struct ether_header *)packet;
    fl_eth_type_t *eth_type = get_fl_eth_type(ntohs(eth_header->ether_type));

    json_object_object_add(jobj, "timestamp_2", json_object_new_double(t_sec + (t_usec/1000000)));
    json_object_object_add(jobj, "length.caplen", json_object_new_int(packet_header.caplen));
    json_object_object_add(jobj, "length.total", json_object_new_int(packet_header.len));
    json_object_object_add(jobj, "ethernet.type.code", json_object_new_string(eth_type->short_code));
    json_object_object_add(jobj, "ethernet.type.value", json_object_new_int64(ntohs(eth_header->ether_type)));
    json_object_object_add(jobj, "ethernet.type.desc", json_object_new_string(eth_type->type_desc));
    json_object_object_add(jobj, "log.level", json_object_new_string("INFO"));

    printf("%s\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));

    json_object_put(jobj);
}

static void frame_logger_packet_handler(unsigned char *args, const struct pcap_pkthdr *packet_header,
                        const unsigned char *packet_body)
{
    print_packet_info(packet_body, *packet_header);
}

int processor_packet_stream(char *net_device)
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_create(net_device, error_buffer);
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

    if (pcap_activate(handle) != 0) {
        fprintf(stderr, "Error activating packet capture handle: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        return 1;
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

    int ret = pcap_set_tstamp_precision(handle, PCAP_TSTAMP_PRECISION_NANO);
    if (ret != 0) {
        log_error("Error activating Nanosecond percision");
    }

    pcap_loop(handle, 0, frame_logger_packet_handler, NULL);

    pcap_close(handle);

    return 0;
}
