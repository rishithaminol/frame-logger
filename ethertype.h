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

#ifndef ETHERTYPE_H
#define ETHERTYPE_H

#include <sys/types.h>

typedef struct fl_eth_type {
    u_int16_t   type_value;
    char        *type_desc;
    char        short_code[256];
    int         range;             /* This will become 1, If type_value is a range  */
    u_int16_t   type_value_start;
    u_int16_t   type_value_end;
} fl_eth_type_t;

extern fl_eth_type_t *get_fl_eth_type(u_int16_t eth_type_);

#endif
