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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "processor.h"
#include "packet_chain.h"

int main(int argc, char *argv[])
{
    int count;
    int i = 0;
    packet_link_t *link;

    if (argc < 3) {
        fprintf(stderr, "Please provide file name\n");
        exit(1);
    }

    count = atoi(argv[2]);
    if (count == 0) {
        fprintf(stderr, "Please insert valid integer 2nd argument.\n");
        exit(1);
    }

    packet_chain_t *chain = new_packet_chain();
    processor_packet_stream(argv[1], chain, STREAM_FILE);

    /**
     * @brief This loop checks whether the processor accurately captures the
     *          number of packets described by `count`
    */
    while ((link = packet_chain_pop(chain)) != NULL) {
        free_packet_link(link);
        i++;
    }

    printf("Captured packets: %d\n", i);

    free_packet_chain(chain);

    if (i != count){
        fprintf(stderr, "Desired number of packets and captured packet count does not match\n");
        return 1;
    }

    return 0;
}
