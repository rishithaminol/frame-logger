#ifndef UTILS_H
#define UTILS_H

typedef enum frame_logger_opts {
    STREAM_NET_INTERFACE = 1 << 0,
    /** @brief `STREAM_FILE` can be used to standard input as well. */
    STREAM_FILE = 1 << 1
} frame_logger_opts_t;

extern pcap_t *refined_pcap_handle(char *source, frame_logger_opts_t frame_logger_opts);
extern int apply_pcap_filter(pcap_t *handle, char *source, char *filter_exp);

#endif