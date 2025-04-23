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
