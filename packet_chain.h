/*
 * This file is part of frame-logger.
 * Copyright (C) 2025 Rishitha Minol
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
