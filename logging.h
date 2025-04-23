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
