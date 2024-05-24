#include "ft_ping.h"

void parsing_arguments(int argc, char *argv[]) {
    int verbose = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-?") == 0) {
            print_help();
            exit(EXIT_SUCCESS);
        } else if (argv[i][0] == '-') {
            printf("./ft_ping: unrecognized option '%s'\n", argv[i]);
            printf("Try 'ping -?' for more information.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (argc <= 1) {
        print_no_args();
        exit(EXIT_FAILURE);
    }

    char *destination = argv[argc - 1];
    printf("Destination: %s\n", destination);

    if (verbose) {
        printf("Verbose mode enabled \n");
    }
}


void print_no_args() {
    printf("./ft_ping: missing host operand\n");
    printf("Try 'ping -?' for more information.\n");
}

void print_unrecognized_option(const char *option) {
    printf("./ft_ping: unrecognized option '%s'\n", option);
    printf("Try 'ping --help' or 'ping --usage' for more information.\n");
}

void print_help() {
    printf("Usage: ./ft_ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
    printf(COLOR_GREEN "Options controlling ICMP request types:\n\n" COLOR_RESET);
    printf("        ...not at the moment.\n\n");
    printf(COLOR_GREEN"Options valid for all request types:\n\n"COLOR_RESET);
    printf("-v             verbose output\n\n");
    printf(COLOR_GREEN"Options valid for echo requests:\n\n"COLOR_RESET);
    printf("-?                 give this help list\n\n");
    // printf("-V, --version              print program version\n\n");
    printf("Report bugs to <bug-mpagani@gnu.org>.");
}
