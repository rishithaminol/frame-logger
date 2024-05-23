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
