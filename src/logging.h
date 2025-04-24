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
