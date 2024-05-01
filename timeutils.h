#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <time.h>

extern int timespec_iso8601(char *buf, int len, struct timespec *ts);
extern void set_timezone(char *zone);

#endif
