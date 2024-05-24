#include "ft_ping.h"

void parsing_arguments(int argc, char *argv[])
{
    int opt;
    int verbose = 0;

    while ((opt = getopt(argc, argv, "v?")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case '?':
            default:
                print_usage();
                exit(EXIT_SUCCESS);
        }
    }
    
    if (optind >= argc) {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    char *destination = argv[optind];
    printf("Destination: %s\n", destination);

    if (verbose) {
        printf("Verbose mode enabled \n");
    }



}

void print_usage() {
    printf("Usage: ft_ping [-v] [-?] destination\n");
    printf("Options:\n");
    printf("  -v       : Verbose mode\n");
    printf("  -?       : Display usage information\n");
}