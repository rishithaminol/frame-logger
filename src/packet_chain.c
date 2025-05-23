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

/**
 * @file packet_chain.c
 * 
 * This section will handle packets as a chain. This section accurately organize
 * packets captured by pcap_loop. Handler functions will utilize functions
 * and data types defined in this section. This section act as a preassure
 * balancing for put/pop interactions with \ref packet_chain_t. While pcap_loop
 * stores data as soon as it recives and later we can process anything stored in
 * \ref packet_chain_t. With this mechanism we can optimize data fetching and
 * outputting paralalley.
 * 
 * The design of packet chain is to quickly copy
 * pcap captured data as soon as possible without interruptig pcap_loop
 * (holding). If we interrupt, data buffering cost will be at pcap_loop
 * hand. As soon as pcap_loop callback function triggers it quickly copy
 * data into packet_chain structure. Then it triggers
 * \ref packet_chain_t->trigger_cond.
 * 
 * \ref packet_chain_t is a linked list of packet_link(s). There are 3 states
 * of a \ref packet_chain_t.
 * 1. Packet chain does not have any link (\ref packet_chain_t.head and
 *      \ref packet_chain_t.tail are NULL)
 * 2. Packet chain have only one link (\ref packet_chain_t.head and
 *      \ref packet_chain_t.tail points to the same link)
 * 3. Packet chain have more than one link (\ref packet_chain_t.head and
 *      \ref packet_chain_t.tail have different links)
*/

#include <stdio.h>
#include <pcap/pcap.h>
#include <time.h>
#include <string.h>

#include "packet_chain.h"
#include "logging.h"

void free_packet_link(packet_link_t *packet_link)
{
    if (packet_link != NULL) {
        free(packet_link->packet);
        free(packet_link->packet_header);
        free(packet_link);
    }
}

void free_packet_chain(packet_chain_t *chain)
{
    pthread_mutex_destroy(&chain->lock);
    pthread_mutex_destroy(&chain->trigger_lock);
    pthread_cond_destroy(&chain->trigger_cond);

    free(chain);
}

static packet_link_t *gen_packet_link_(const struct pcap_pkthdr *pkt_hdr,
                                       const uint8_t *packet)
{
    packet_link_t *pkt_link;

    pkt_link = (packet_link_t *)malloc(sizeof(packet_link_t));
    if (pkt_link == NULL) {
        log_error("Memory allocation error");
        exit(1);
    }

    pkt_link->next = NULL;

    pkt_link->packet_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));
    pkt_link->packet_header->caplen = pkt_hdr->caplen;
    pkt_link->packet_header->len = pkt_hdr->len;
    pkt_link->packet_header->ts.tv_sec = pkt_hdr->ts.tv_sec;
    pkt_link->packet_header->ts.tv_usec = pkt_hdr->ts.tv_usec;

    // log_info("Packet length: %d", pkt->packet_header->len); ////
    pkt_link->packet = (uint8_t *)malloc(pkt_hdr->len);
    memcpy(pkt_link->packet, packet, pkt_link->packet_header->len);

    return pkt_link;
}

/**
 * @brief Add packet_link to the the given packet_chain
 * 
 * @param[pkt_hdr] PCAP generated packet header
 * @param[packet]  PCAP generated packet
 * @param[chain]   Packet chain `struct packet_chain`
 * 
 * This function copy the contents of packet header (`pkt_hdr`) and 
 * `packet` content into new memory allocated variables.
 * A new `struct packet_link` type will be created. The newly created
 * `packet_link` type will then be inserted into `chain`
*/
void packet_chain_put(const struct pcap_pkthdr *pkt_hdr,
                      const uint8_t *packet,
                      packet_chain_t *chain)
{
    packet_link_t *pkt;

    /**
     * Later bring the execution logic of this function into 
     * here.
    */
    pkt = gen_packet_link_(pkt_hdr, packet);

    pthread_mutex_lock(&chain->lock);

    if (chain->head == NULL) {
        chain->head = pkt;
        chain->tail = chain->head;
        goto close_lock;
    }
    /* beyond this head != NULL */

    /* attach new header */
    chain->head->next = pkt;
    chain->head = chain->head->next;

close_lock:
    pthread_mutex_unlock(&chain->lock);
}

packet_link_t *packet_chain_pop(packet_chain_t *chain)
{
    packet_link_t *ret;

    if (chain == NULL)
        return NULL;

    pthread_mutex_lock(&(chain->lock));

    if (chain->head == NULL) {
        ret = NULL;
        goto close_lock;
    }
    /* beyond this head != NULL */

    ret = chain->tail;
    chain->tail = ret->next;

    if (chain->tail == NULL)
        chain->head = NULL;

close_lock:
    pthread_mutex_unlock(&(chain->lock));

    return ret;
}

packet_chain_t *new_packet_chain(void)
{
    packet_chain_t *chain = malloc(sizeof(packet_chain_t));

    pthread_mutex_init(&chain->lock, NULL);
    pthread_mutex_init(&chain->trigger_lock, NULL);
    pthread_cond_init(&chain->trigger_cond, NULL);
    chain->trigger_is = inactive;

    chain->head = NULL;
    chain->tail = NULL;

    if (chain == NULL) {
        log_error("Memory allocation error. new_packet_chain");
    }

    return chain;
}
