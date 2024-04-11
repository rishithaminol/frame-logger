/** @file frame-logger.c
* @brief Frame logger. A packet capturing tool with enhanced inteligence.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <pcap/pcap.h>

void print_packet_info(const uint8_t *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

int get_packet(char *interf_name) {
    char *device = interf_name;
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const uint8_t *packet;
    struct pcap_pkthdr packet_header;
    int packet_count_limit = 1;
    int timeout_limit = 10; /* In milliseconds */

    /* device = pcap_lookupdev(error_buffer);
    if (device == NULL) {
        printf("Error finding device: %s\n", error_buffer);
        return 1;
    } */
    printf("Reading device %s\n", device);

    /* Open device for live capture */
    handle = pcap_open_live(
            device,
            BUFSIZ,
            packet_count_limit,
            timeout_limit,
            error_buffer
        );

     /* Attempt to capture one packet. If there is no network traffic
      and the timeout is reached, it will return NULL */
     packet = pcap_next(handle, &packet_header);
     if (packet == NULL) {
        printf("No packet found.\n");
        return 2;
    }

    /* Our function to output some info */
    print_packet_info(packet, packet_header);

    return 0;
}

void usage(void)
{
    printf("Usage:\n"
           "\t-r,--read\t\tRead from file. Use '-' for stdin\n");
}

int main(int argc, char *argv[])
{
    int opt_key;
    int option_index = 0;
    struct option long_options[] = {
        {"read", required_argument, NULL, 'r'},
        {"interface", required_argument, NULL, 'i'},
        // {"output-file", required_argument, NULL, 'o'},
        // {"verbose", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };


    while ((opt_key = getopt_long(argc, argv, "r:i:v", long_options, &option_index)) != -1) {
        switch (opt_key) {
            case 'r':
                printf("Input file: %s\n", optarg);
                break;
            case 'i':
                get_packet(optarg);
                break;
            case '?':
                usage();
                return 1;
            default:
                printf("Hello world\n");
                break;
        }
    }

    return 0;
}
