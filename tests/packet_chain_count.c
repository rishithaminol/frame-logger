#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "processor.h"
#include "packet_chain.h"

int main(int argc, char *argv[])
{
    int count;
    int i = 0;
    packet_link_t *link;

    if (argc < 3) {
        fprintf(stderr, "Please provide file name\n");
        exit(1);
    }

    count = atoi(argv[2]);
    if (count == 0) {
        fprintf(stderr, "Please insert valid integer 2nd argument.\n");
        exit(1);
    }

    packet_chain_t *chain = new_packet_chain();
    processor_packet_stream(argv[1], chain, STREAM_FILE);

    /**
     * @brief This loop checks whether the processor accurately captures the
     *          number of packets described by `count`
    */
    while ((link = packet_chain_pop(chain)) != NULL) {
        free_packet_link(link);
        i++;
    }

    printf("Captured packets: %d\n", i);

    free_packet_chain(chain);

    if (i != count){
        fprintf(stderr, "Desired number of packets and captured packet count does not match\n");
        return 1;
    }

    return 0;
}
