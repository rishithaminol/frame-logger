/*
 * This file is part of frame-logger project.
 * Copyright (C) 2025 Rishitha Minol
 *
 * This file is dual-licensed:
 *
 * - Under the GNU General Public License v3.0 or later (GPLv3),
 *   as published by the Free Software Foundation; and
 *
 * - Under the Business Source License 1.1 (BSL),
 *   as published by MariaDB Corporation.
 *
 * You may choose either license for use, subject to the terms below.
 *
 * GPLv3: See the LICENSE.gpl file in the project root or
 *        <https://www.gnu.org/licenses/gpl-3.0.html>
 *
 * BSL:  See the LICENSE.bsl file in the project root or
 *       <https://mariadb.com/bsl11>
 *
 * Under the BSL, production/commercial use of this file is restricted
 * until April 23, 2028. After that date, this file will be permanently
 * licensed under the terms of GPLv3.
 *
 * For licensing inquiries, contact: rishithaminol@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <json.h>
#include <string.h>

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

#define IP_STR_MAX_LEN 46 /*!< Maximum character length if IP stored in a variable v4/v6 */

/**
 * @todo This function should have nested json structuring.
 * @todo Create unit tests to accurately check nano second timestamping. This should be
 *          an intergrated test which connect to Elasticsearch and do the testing.
 * @todo Most of the time protocol is decided by the port number. Sometimes
 *          protocol is decided by reading the IP packet data. And sometimes
 *          other techniques can also be used. So we need to specifically output which
 *          technique is used to identify the protocol.
 * @todo We should output TTL of each packet if it can be found.
*/
static void print_packet_info(packet_link_t *pkt_link)
{
    char src_mac[18], dst_mac[18];
    struct json_object *jobj = json_object_new_object();
    char ip_protocol[10];
    uint16_t eth_type;
    char src_ip[IP_STR_MAX_LEN], dst_ip[IP_STR_MAX_LEN];

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

    eth_type = ntohs(eth_header->ether_type);
    ether_ntoa_r((struct ether_addr *)eth_header->ether_shost, src_mac);
    ether_ntoa_r((struct ether_addr *)eth_header->ether_dhost, dst_mac);
    fl_eth_type_t *fl_eth_type = get_fl_eth_type(eth_type);

    json_object_object_add(jobj, "timestamp_", json_object_new_string(timestr));
    json_object_object_add(jobj, "packet_header.caplen", json_object_new_int(packet_header->caplen));
    json_object_object_add(jobj, "packet_header.len", json_object_new_int(packet_header->len));
    json_object_object_add(jobj, "ethernet.type.code", json_object_new_string(fl_eth_type->short_code));
    json_object_object_add(jobj, "ethernet.type.value", json_object_new_int64(ntohs(eth_header->ether_type)));
    json_object_object_add(jobj, "ethernet.type.desc", json_object_new_string(fl_eth_type->type_desc));
    json_object_object_add(jobj, "ethernet.addr.dst", json_object_new_string(dst_mac));
    json_object_object_add(jobj, "ethernet.addr.src", json_object_new_string(src_mac));
    json_object_object_add(jobj, "log.level", json_object_new_string("INFO"));

    /* Extract IPv4 header */
    /* References:
     *  https://litux.nl/mirror/snortids/0596006616/snortids-CHP-2-SECT-2.html
     **/
    if (eth_type == ETHERTYPE_IP) {
        struct ip *ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        strcpy(src_ip, inet_ntoa(ip_header->ip_src));
        strcpy(dst_ip, inet_ntoa(ip_header->ip_dst));


        // Convert ip_off from network byte order to host byte order
        uint16_t frag_field = ntohs(ip_header->ip_off);

        // Extract flags
        int reserved_flag = (frag_field & IP_RF) >> 15;
        int dont_fragment = (frag_field & IP_DF) >> 14;
        int more_fragments = (frag_field & IP_MF) >> 13;

        int fragment_offset = frag_field & IP_OFFMASK;

        json_object_object_add(jobj, "ip.frag.rf", json_object_new_int(reserved_flag));
        json_object_object_add(jobj, "ip.frag.df", json_object_new_int(dont_fragment));
        json_object_object_add(jobj, "ip.frag.mf", json_object_new_int(more_fragments));
        json_object_object_add(jobj, "ip.frag.offset", json_object_new_int(fragment_offset));

        switch(ip_header->ip_p) {
            case IPPROTO_ICMP:
                strcpy(ip_protocol, "ICMP");
                break;
            case IPPROTO_TCP:
                strcpy(ip_protocol, "TCP");
                break;
            case IPPROTO_UDP:
                strcpy(ip_protocol, "UDP");
                break;
            default:
                strcpy(ip_protocol, "UNKNOWN");
        }

        json_object_object_add(jobj, "ip.src", json_object_new_string(src_ip));
        json_object_object_add(jobj, "ip.dst", json_object_new_string(dst_ip));
        json_object_object_add(jobj, "ip.header_length", json_object_new_int(ip_header->ip_hl * 4));
        json_object_object_add(jobj, "ip.protocol", json_object_new_string(ip_protocol));
        json_object_object_add(jobj, "ip.total_length", json_object_new_int(ntohs(ip_header->ip_len)));
        json_object_object_add(jobj, "ip.frag.id", json_object_new_int(ntohs(ip_header->ip_id))); /* Fragmentation ID */
        json_object_object_add(jobj, "ip.checksum", json_object_new_int(ntohs(ip_header->ip_sum)));
        json_object_object_add(jobj, "ip.ttl", json_object_new_int(ip_header->ip_ttl));
        json_object_object_add(jobj, "ip.tos", json_object_new_int(ip_header->ip_tos)); /* Type of Service */
    }

    printf("%s\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));

    json_object_put(jobj);
}

/**
 * @todo Implement network session identification with the duration
*/
static void packet_chain_handler(unsigned char *user,
                                  const struct pcap_pkthdr *packet_header,
                                  const uint8_t *packet_body)
{
    /**
     * @brief packet chain structure
     */
    packet_chain_t *pkt_chain = (packet_chain_t *)user;

    packet_chain_put(packet_header, packet_body, pkt_chain);

    /** @todo In future we have to design a function to process extracted data in
     * \p pkt_chain. In this section we basically trigger the printer. But before
     * triggering \ref packet_printer_thread we have to process captured data.
     * Then \ref packet_printer_thread should come after that.
     * The \ref packet_chain_t.trigger_lock is designed for triggering the
     * processing part of captured data. But for now it is used to trigger
     * \ref packet_printer_thread. The idea here is to trigger \ref packet_chain_t data
     * processor and trigger \ref packet_printer_thread by that processor
     */

    // Signal to the packet printer thread to continue
    pthread_mutex_lock(&pkt_chain->trigger_lock);
    if (pkt_chain->trigger_is == inactive)
        pthread_cond_signal(&pkt_chain->trigger_cond);
    pthread_mutex_unlock(&pkt_chain->trigger_lock);
}

/**
 * @brief This function call the packet printing function keeping the conditional
 *        waiting based on \ref packet_chain_handler input
 * 
 * @todo Move this function into \ref packet_chain.c section. I feel that this should
 *       reside in that section as always running thread or another mechanism.
 *       We should not print things as JSON without formatting correctly.
 * 
 * This function runs as a thread and it will be initialized by
 *  @ref packet_chain_processor. After that, this will be periodically triggered
 *  when packets available on the packet chain queue.
*/
static void *packet_printer_thread(void *arg)
{
    packet_chain_t *chain = (packet_chain_t *)arg;
    packet_link_t *link = NULL;

    while (1) {
        while ((link = packet_chain_pop(chain)) != NULL) {
            print_packet_info(link);
            free_packet_link(link);
        }

        pthread_mutex_lock(&chain->trigger_lock);
        chain->trigger_is = inactive;
        pthread_cond_wait(&chain->trigger_cond, &chain->trigger_lock);
        chain->trigger_is = active;
        pthread_mutex_unlock(&chain->trigger_lock);
    }

    return NULL;
}


int packet_chain_processor(pcap_t *handle, packet_chain_t *chain, bool json_log)
{
    pthread_t thrd1;

    if (json_log)
        pthread_create(&thrd1, NULL, packet_printer_thread, chain);

    pcap_loop(handle, 0, packet_chain_handler, (unsigned char *)chain);
    pcap_close(handle);

    if (json_log)
        pthread_join(thrd1, NULL);

    return 0;
}
