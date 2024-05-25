#include "ft_ping.h"


void parsing_arguments(int argc, char *argv[], cmd_args_t *cmd_args) {
    // Initialize the struct fields
    cmd_args->verbose = false;
    cmd_args->show_help = false;
    cmd_args->hostname = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            cmd_args->verbose = true;
        } else if (strcmp(argv[i], "-?") == 0) {
            cmd_args->show_help = true;
            print_help();
            exit(EXIT_SUCCESS);
        } else if (argv[i][0] == '-') {
            print_unrecognized_option(argv[i]);
            exit(EXIT_FAILURE);
        } else {
            cmd_args->hostname = argv[i];
        }
    }

    if (cmd_args->hostname == NULL) {
        print_no_args();
        exit(EXIT_FAILURE);
    }

    if (cmd_args->verbose) {
        printf("Verbose mode enabled\n");
    }

    printf("Destination: %s\n", cmd_args->hostname);
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
