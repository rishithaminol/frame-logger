/**
 * @file timestamping.c
 * @brief This is a test file for timestamping
 * 
 * Include timestamping section of the program and compile this.
 * Then test whether that functionality works perfectly.
*/

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "timeutils.h"

int iso8601_timestamp(struct timespec *ts)
{
    const unsigned int TIME_FMT_LEN = strlen("2019-04-21T03:30:00.000000000Z") + 1;
    char timestr[32];

    if (timespec_iso8601(timestr, sizeof(timestr), ts) != 0) {
        printf("timespec2str failed!\n");
        return EXIT_FAILURE;
    } else {
        printf("Time: %s\n", timestr);
        return EXIT_SUCCESS;
    }
}


int main(void)
{
    struct timespec tms;
    int64_t nanos, nanos_part;

    if(clock_gettime(CLOCK_REALTIME, &tms)) {
        return -1;
    }

    nanos = tms.tv_sec * 1000000000;
    printf("timestamp: %" PRIu64 "\n", nanos);

    nanos_part = tms.tv_nsec;
    printf("timestamp          : %" PRIu64 "\n", nanos_part);

    nanos += nanos_part;
    printf("timestamp: %" PRIu64 "\n", nanos);

    iso8601_timestamp(&tms);

    return 0;
}
