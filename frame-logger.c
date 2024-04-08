/** @file frame-logger.c
* @brief Frame logger. A packet capturing tool with enhanced inteligence.
*/

#include <stdio.h>
#include <pcap/pcap.h>

#include "module.h"


int main(void)
{
    printf("Hello World\n");
    printf("pcap lib version: %s\n", pcap_lib_version());
    call_internal_function();

    return 0;
}
