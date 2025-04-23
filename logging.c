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
#include <stdarg.h>
#include <json.h>
#include <time.h>
#include <string.h>

#include "logging.h"
#include "timeutils.h"

/**
 * @brief This is inserted as an argument into \ref log_common_components function.
*/
typedef enum LOG_LEVEL {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
} log_level_t;

/**
 * @brief `log_common_components` adds common fields to the message body.
 * 
 * This function feeds following details into the message fields
 * - Timestamp
 * - Log Level
*/
static void log_common_components(struct json_object *jobj, const char *message, log_level_t ll)
{
    struct timespec ts;
    char timestr[32];

    strcpy(timestr, "TIMESTAMP_NULL");

    if (clock_gettime(CLOCK_REALTIME, &ts) != -1) {
        if (timespec_iso8601(timestr, sizeof(timestr), &ts) != 0) {
            strcpy(timestr, "timestamping error");
        }
    }

    json_object_object_add(jobj, "timestamp_", json_object_new_string(timestr));
    json_object_object_add(jobj, "message", json_object_new_string(message));

    switch (ll) {
        case DEBUG:
            json_object_object_add(jobj, "log.level", json_object_new_string("DEBUG"));
            break;
        case INFO:
            json_object_object_add(jobj, "log.level", json_object_new_string("INFO"));
            break;
        case WARNING:
            json_object_object_add(jobj, "log.level", json_object_new_string("WARNING"));
            break;
        case ERROR:
            json_object_object_add(jobj, "log.level", json_object_new_string("ERROR"));
            break;
        case CRITICAL:
            json_object_object_add(jobj, "log.level", json_object_new_string("CRITICAL"));
            break;
    }

    printf("%s\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));
    json_object_put(jobj);
}

void log_info(const char *format, ...)
{
    va_list args;
    char string_buffer[LOGGING_BUFFER_SIZE];
    struct json_object *jobj = json_object_new_object();

    va_start(args, format);
    vsnprintf(string_buffer, LOGGING_BUFFER_SIZE, format, args);

    log_common_components(jobj, string_buffer, INFO);
    va_end(args);
}

void log_error(const char *format, ...)
{
    va_list args;
    char string_buffer[LOGGING_BUFFER_SIZE];
    struct json_object *jobj = json_object_new_object();

    va_start(args, format);
    vsnprintf(string_buffer, LOGGING_BUFFER_SIZE, format, args);

    log_common_components(jobj, string_buffer, ERROR);
    va_end(args);
}
