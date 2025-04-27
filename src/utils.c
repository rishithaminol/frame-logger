#include <stdio.h>
#include <pcap/pcap.h>

#include "utils.h"
#include "logging.h"

pcap_t *refined_pcap_handle(char *source, frame_logger_opts_t frame_logger_opts)
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;

    if (frame_logger_opts & STREAM_NET_INTERFACE) {
        handle = pcap_create(source, error_buffer);
        if (handle == NULL) {
            fprintf(stderr, "Error creating packet capture handle: %s\n", error_buffer);
            return NULL;
        }

        if (pcap_set_promisc(handle, 1) != 0) {
            fprintf(stderr, "Error setting promiscuous mode\n");
            pcap_close(handle);
            return NULL;
        }

        if (pcap_set_snaplen(handle, 65535) != 0) {
            fprintf(stderr, "Error setting snap length\n");
            pcap_close(handle);
            return NULL;
        }

        if (pcap_set_timeout(handle, 10) != 0) {
            fprintf(stderr, "Error setting capture timeout\n");
            pcap_close(handle);
            return NULL;
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
            return NULL;
        }

        if (pcap_activate(handle) != 0) {
            fprintf(stderr, "Error activating packet capture handle: %s\n", pcap_geterr(handle));
            pcap_close(handle);
            return NULL;
        }
    } else if (frame_logger_opts & STREAM_FILE) {
        // handle = pcap_open_offline(source, error_buffer);
        handle = pcap_open_offline_with_tstamp_precision(source,
                                PCAP_TSTAMP_PRECISION_NANO,
                                error_buffer);
        if (handle == NULL) {
            printf("Error opening file: %s\n", error_buffer);
            return NULL;
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
            log_error("Some other value");
    }

    /* Currently this program supports only Ethernet card types.
     * New formats will be included in the future if possible.
     */
    if (pcap_datalink(handle) != DLT_EN10MB) {
        log_error("This program only supports Ethernet cards!");
        pcap_close(handle);
        return NULL;
    }

    return handle;
}

int apply_pcap_filter(pcap_t *handle, char *source, char *filter_exp)
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;      // The compiled filter expression
    bpf_u_int32 net = 0;            // IP of our device (not used deeply here)
    bpf_u_int32 mask;

    if (pcap_lookupnet(source, &net, &mask, error_buffer) == -1) {
        fprintf(stderr, "Can't get netmask for device %s\n", source);
        net = 0;
        mask = 0;
    }

    // 3. Compile the filter
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 2;
    }

    // 4. Apply the filter
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 2;
    }

    return 0;
}
