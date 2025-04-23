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
 * @file timestamping.c
 * @brief This is a test file for timestamping
 * 
 * Include timestamping section of the program and compile this.
 * Then test whether that functionality works perfectly.
 * 
 * Generate hand made timestamp.
 * Pass the timestamp into timespec_iso8601 and get the output string.
 * Compare the integers with the returned string.
 * If it equals return 0
*/

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "timeutils.h"

int main(void)
{
    char timestr[32];
    struct timespec tms1, tms2;
    int ret = 0;

    tms1.tv_sec = 1555817400;
    tms1.tv_nsec = 846599595;

    tms2.tv_sec = 1555817400;
    tms2.tv_nsec = 44817714;

    /*
    This section used to visualize how timestamping data is displayed.

    nanos = tms.tv_sec * 1000000000;
    printf("timestamp: %" PRIu64 "\n", nanos);

    nanos_part = tms.tv_nsec;
    printf("timestamp          : %" PRIu64 "\n", nanos_part);

    nanos += nanos_part;
    printf("timestamp: %" PRIu64 "\n", nanos);
    */

    set_timezone("UTC");

    if (timespec_iso8601(timestr, sizeof(timestr), &tms1) != 0) {
        printf("timespec_iso8601 failed!\n");
        return EXIT_FAILURE;
    }

    if (strcmp("2019-04-21T03:30:00.846599595Z", timestr) != 0)
        ret = 1;

    if (timespec_iso8601(timestr, sizeof(timestr), &tms2) != 0) {
        printf("timespec_iso8601 failed!\n");
        return EXIT_FAILURE;
    }

    if (strcmp("2019-04-21T03:30:00.044817714Z", timestr) != 0)
        ret = 1;

    return ret;
}
