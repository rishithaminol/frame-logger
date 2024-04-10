/** @file frame-logger.c
* @brief Frame logger. A packet capturing tool with enhanced inteligence.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <pcap.h>

int net_interfaces(void)
{
    int i = 0;
    pcap_if_t *interfaces, *temp;
    char error[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&interfaces, error) == PCAP_ERROR) {
        printf("Error in pcap findall devs\n");
        return -1;
    }

    printf("The interfaces present on the system are:\n");
    for (temp = interfaces; temp; temp = temp->next) {
        printf("%d : %s\n", i++, temp->name);

        if (temp->description != NULL)
            printf("%s\n", temp->description);
    }

    pcap_freealldevs(interfaces);

    return 0;
}

void usage(void)
{
    printf("Usage:\n"
           "\t-r,--read\t\tRead from file. Use '-' for stdin\n");
}

int main(int argc, char *argv[])
{
    int opt_key;
    int option_index = 0;
    struct option long_options[] = {
        {"read", required_argument, NULL, 'r'},
        // {"output-file", required_argument, NULL, 'o'},
        // {"verbose", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    net_interfaces();

    while ((opt_key = getopt_long(argc, argv, "r:v", long_options, &option_index)) != -1) {
        switch (opt_key) {
            case 'r':
                printf("Input file: %s\n", optarg);
                break;
            case '?':
                usage();
                return 1;
            default:
                printf("Hello world\n");
                break;
        }
    }

    return 0;
}
