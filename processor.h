#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "packet_chain.h"

typedef enum processor_opt {
    STREAM_NET_INTERFACE = 1 << 0,
    /** @brief `STREAM_FILE` can be used to standard input as well. */
    STREAM_FILE = 1 << 1,
    JSON_LOGGER = 1 << 2
} processor_opt_t;

extern int processor_packet_stream(char *source, packet_chain_t *chain,
                            processor_opt_t stream);

#endif
