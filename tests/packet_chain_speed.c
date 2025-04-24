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

/** @file packet_chain_speed.c
 * 
 * Alwasy this program get it's input from stdin.
 * This specific test case checks whether the packet_chain_t is defensive for
 *  parallel access.
 * 
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "processor.h"
#include "packet_chain.h"

int packet_count = 0;
int endles_loop = 1;

void *packet_stream_(void *chain)
{
    packet_chain_t *x = (packet_chain_t *)chain;

    processor_packet_stream("-", x, STREAM_FILE);

    return NULL;
}

/** @brief endless loop */
void *count_packets_(void *chain)
{
    packet_chain_t *x = (packet_chain_t *)chain;
    packet_link_t *link = NULL;

    while (endles_loop) {
        while ((link = packet_chain_pop(x)) != NULL) {
            free_packet_link(link);
            packet_count++;
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int count;
    pthread_t thrd1, thrd2;

    if (argc < 2) {
        fprintf(stderr, "Please provide desired packet count\n");
        exit(1);
    }

    count = atoi(argv[1]);
    if (count == 0) {
        fprintf(stderr, "Please insert valid integer 1st argument.\n");
        exit(1);
    }

    packet_chain_t *chain = new_packet_chain();

    pthread_create(&thrd1, NULL, count_packets_, chain);
    pthread_create(&thrd2, NULL, packet_stream_, chain);
    pthread_join(thrd2, NULL);
    

    // Verify desired amount of entries. Try 3 times. sleeping 1s
    for (int i = 0; i < 3; i++) {
        if (packet_count != count)
            sleep(1);
    }

    endles_loop = 0;
    pthread_join(thrd1, NULL);
    free_packet_chain(chain);

    if (packet_count != count){
        fprintf(stderr, "Desired number of packets and captured packet count does not match\n");
        return 1;
    }

    return 0;
}
