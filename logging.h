/**
 * @file logging.h
 * @brief Logging facility.
 * 
 * Entire logging facility follows adaptively modified version of
 *   [ECS](https://www.elastic.co/guide/en/ecs/current/index.html)
 *   standard since this tool use Elasticsearch and Kibana
 *   for deper analyzation of network activity. 
 */

#ifndef LOGGING_H
#define LOGGING_H

/**
 * @brief This determines the size of logging buffer size.
 * 
 * When logging functions perform their output in JSON,
 *  formatted string which comes into log functions such as @ref log_info
 *  will be copied into a single variable before inserted into a JSON field.
*/
#define LOGGING_BUFFER_SIZE 1024

extern void log_info(const char *format, ...);
extern void log_error(const char *format, ...);

#endif
