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

  while ((opt_key = getopt_long(argc, argv, "r:v", long_options, &option_index)) != -1) {
    switch (opt_key) {
      case 'r':
        printf("Input file: %s\n", optarg);
        break;
      case '?':
        usage();
        return 1;
      default:
        break;
    }
  }

  return 0;
}
