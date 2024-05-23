/** @file frame-logger.c
* @brief Frame logger. A packet capturing tool with enhanced inteligence.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pcap/pcap.h>

#include <json.h>

#include "logging.h"
#include "processor.h"
#include "timeutils.h"
#include "packet_chain.h"
#include "prog_version.h"


void usage(void)
{
    printf("Usage:\n"
           "\t-r,--read\t\tRead from file. Use '-' for stdin\n"
           "\t-i,--interface\t\tNetwork inteface for monitoring traffic\n"
           "\t--timezone\t\tSet timezone related data\n"
           "\t--version\t\tProgram version");
}

/**
 * @todo configure `--timezone` option
*/
int main(int argc, char *argv[])
{
    int opt_key;
    int option_index = 0;
    struct option long_options[] = {
        {"read", required_argument, NULL, 'r'},
        {"interface", required_argument, NULL, 'i'},
        {"version", no_argument, NULL, 'v'},
        // {"output-file", required_argument, NULL, 'o'},
        // {"verbose", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };
    packet_chain_t *chain = new_packet_chain();

    set_timezone("UTC");

    while ((opt_key = getopt_long(argc, argv, "r:i:v", long_options, &option_index)) != -1) {
        switch (opt_key) {
            case 'r':
                processor_packet_stream(optarg, chain,
                                        STREAM_FILE | JSON_LOGGER);
                break;
            case 'i':
                processor_packet_stream(optarg, chain,
                                        STREAM_NET_INTERFACE | JSON_LOGGER);
                break;
            case 'v':
                printf("%s version: %d.%d.%d\n", argv[0],
                                       PROJECT_VERSION_MAJOR,
                                       PROJECT_VERSION_MINOR,
                                       PROJECT_VERSION_PATCH);
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
