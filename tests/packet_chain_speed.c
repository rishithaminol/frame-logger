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
