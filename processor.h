#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "packet_chain.h"

typedef enum processor_opt {
    STREAM_NET_INTERFACE = 1 << 0,
    STREAM_FILE = 1 << 1,
    STREAM_STDIN = 1 << 2,
    JSON_LOGGER = 1 << 3
} processor_opt_t;

extern int processor_packet_stream(char *source, packet_chain_t *chain,
                            processor_opt_t stream);

#endif
