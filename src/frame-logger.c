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
 * @file frame-logger.c
 * @brief Frame logger. A packet capturing tool with enhanced inteligence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pcap/pcap.h>
#include <stdbool.h>
#include <json.h>

#include "logging.h"
#include "processor.h"
#include "timeutils.h"
#include "packet_chain.h"
#include "prog_version.h"
#include "utils.h"

enum {
    GETOPT_FILTER = 1000,   // start from 1 to avoid clashing with short options
    GETOPT_HELP
};


void usage(void)
{
    printf("Usage:\n"
           "\t-r,--read\t\tRead from file. Use '-' for stdin\n"
           "\t-i,--interface\t\tNetwork inteface for monitoring traffic\n"
           "\t--filter\t\ttcpdump similar filter expression\n"
           "\t--timezone\t\tSet timezone related data\n"
           "\t--version\t\tProgram version\n");
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
        {"filter", required_argument, NULL, GETOPT_FILTER},
        {"version", no_argument, NULL, 'v'},
        // {"output-file", required_argument, NULL, 'o'},
        // {"verbose", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };
    packet_chain_t *chain = new_packet_chain();

    /* Option selection specific variables */
    int use_file = 0;
    int use_interface = 0;
    int use_filter = 0;
    char filter_string[255];
    char pcap_source[20];       /*!< eth0 if net interface or some file path */

    pcap_t *handle = NULL;

    set_timezone("UTC");

    while ((opt_key = getopt_long(argc, argv, "r:i:v", long_options, &option_index)) != -1) {
        switch (opt_key) {
            case GETOPT_FILTER:
                strcpy(filter_string, optarg);
                use_filter = 1;
                break;
            case 'r':
                strcpy(pcap_source, optarg);
                use_file = 1;
                break;
            case 'i':
                strcpy(pcap_source, optarg);
                use_interface = 1;
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
                log_error("parameter handling malfunction.\n");
                break;
        }
    }

    if (use_file == 1 && use_interface == 1) {      /* User is not allowed both */
        printf("Standard input and interface can not be used at the same time\n");
        return 1;
    }

    if (use_file) {
        handle = refined_pcap_handle(pcap_source, STREAM_FILE);
    } else if (use_interface) {
        handle = refined_pcap_handle(pcap_source, STREAM_NET_INTERFACE);
    }

    if (use_filter) {
        log_info("Using filter: %s", filter_string);
        apply_pcap_filter(handle, pcap_source, filter_string);
    }

    packet_chain_processor(handle, chain, true);

    return 0;
}
