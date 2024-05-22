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

typedef struct packet_chain {
    struct packet_link *head;
    struct packet_link *tail;
    pthread_mutex_t lock;
} packet_chain_t;

extern void packet_chain_put(const struct pcap_pkthdr *pkt_hdr,
                             const uint8_t *packet,
                             packet_chain_t *chain);
extern struct packet_link *packet_chain_pop(packet_chain_t *chain);
extern packet_chain_t *new_packet_chain(void);
extern void free_packet_link(packet_link_t *packet_link);
extern void free_packet_chain(packet_chain_t *chain);

#endif
