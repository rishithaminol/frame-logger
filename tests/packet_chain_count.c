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
