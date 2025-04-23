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
