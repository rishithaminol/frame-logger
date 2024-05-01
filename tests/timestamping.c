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

int main(void)
{
    char timestr[32];
    struct timespec tms;
    int64_t nanos, nanos_part;

    if(clock_gettime(CLOCK_REALTIME, &tms)) {
        return -1;
    }

    /*
    Generate timestamp
    pass that into timespec_iso8601 and get the output string
    Compare the integers with the returned string
    If it equals return 0
    */

    nanos = tms.tv_sec * 1000000000;
    printf("timestamp: %" PRIu64 "\n", nanos);

    nanos_part = tms.tv_nsec;
    printf("timestamp          : %" PRIu64 "\n", nanos_part);

    nanos += nanos_part;
    printf("timestamp: %" PRIu64 "\n", nanos);


    if (timespec_iso8601(timestr, sizeof(timestr), &tms) != 0) {
        printf("timespec2str failed!\n");
        return EXIT_FAILURE;
    } else {
        printf("Time: %s\n", timestr);
        return EXIT_SUCCESS;
    }

    return 0;
}
