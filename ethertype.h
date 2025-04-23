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
