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
#include <string.h>
#include <stdlib.h>

#include "timeutils.h"

/**
 * @brief This function converts the given timespec ts into a string
 *          representable time string (iso 8601).
 * @param[buf] Expecting string representation of time.
 * @param[len] The length of the buffer prepared before handing over
 *              this function.
 * @param[ts]  The `timespec` which contains unix timestamp. Whenever you
 *              have unix timestamps, convert it into timespec and input.
 * 
 * This function converts seconds portion `ts.tv_sec` into a `struct tm` type.
 *  Then using that `tm` we can generate year, month, date, hour, minute
 *  and seconds portion of the timestamp string. Finally append nanosecond
 *  portion into the string.
 *
 *  `timespec.tv_sec -> tm -> strftime() -> "2019-04-21T03:30:00"` <br>
 *  `"2019-04-21T03:30:00" + "." + timespec.tv_nsec + "Z"`
 * 
 *  Usually the length is 32 bytes. Which is more logical if we calculate it
 *  using `strlen("2019-04-21T03:30:00.000000000Z") + 1` expression.
*/
int timespec_iso8601(char *buf, int len, struct timespec *ts)
{
    int ret;
    struct tm t;

    if (localtime_r(&(ts->tv_sec), &t) == NULL)
        return 1;

    ret = strftime(buf, len, "%FT%T", &t);
    if (ret == 0)
        return 2;
    len -= ret - 1;

    ret = snprintf(&buf[strlen(buf)], len, ".%09ldZ", ts->tv_nsec);
    if (ret >= len)
        return 3;

    return 0;
}

void set_timezone(char *zone)
{
    setenv("TZ", zone, 1);
    tzset();
}
