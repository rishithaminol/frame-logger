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

#ifndef PACKET_CHAIN_H
#define PACKET_CHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct packet_link packet_link_t;
struct packet_link {
    uint8_t *packet;
    struct pcap_pkthdr *packet_header;

    packet_link_t *next;
};

enum packet_chain_trigger {
    active,
    inactive
};
typedef struct packet_chain {
    struct packet_link *head;
    struct packet_link *tail;
    pthread_mutex_t lock;

    pthread_mutex_t trigger_lock;
    pthread_cond_t  trigger_cond;
    enum packet_chain_trigger trigger_is;
} packet_chain_t;

extern void packet_chain_put(const struct pcap_pkthdr *pkt_hdr,
                             const uint8_t *packet,
                             packet_chain_t *chain);
extern struct packet_link *packet_chain_pop(packet_chain_t *chain);
extern packet_chain_t *new_packet_chain(void);
extern void free_packet_link(packet_link_t *packet_link);
extern void free_packet_chain(packet_chain_t *chain);

#endif
