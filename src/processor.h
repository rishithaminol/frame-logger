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
