#include "ft_ping.h"


void parsing_arguments(int argc, char *argv[]) {
    // Initialize the struct fields
    ping_info.cmd_args.verbose = false;
    ping_info.cmd_args.show_help = false;
    ping_info.cmd_args.hostname = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            ping_info.cmd_args.verbose = true;
        } else if (strcmp(argv[i], "-?") == 0) {
            ping_info.cmd_args.show_help = true;
            print_help();
            exit(EXIT_SUCCESS);
        } else if (argv[i][0] == '-') {
            print_unrecognized_option(argv[i]);
            exit(EXIT_FAILURE);
        } else {
            ping_info.cmd_args.hostname = argv[i];
        }
    }

    if (ping_info.cmd_args.hostname == NULL) {
        print_no_args();
        exit(EXIT_FAILURE);
    }
}

void resolve_hostname(char *argv[]) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    int error = getaddrinfo(ping_info.cmd_args.hostname, NULL, &hints, &res);
    if (error != 0) {
        printf("%s: unknown host\n", argv[0]);
        exit(1);
    }


    ping_info.dns_resolution.dest_addr.sin_family = AF_INET;
    ping_info.dns_resolution.dest_addr.sin_port = 0;
    ping_info.dns_resolution.dest_addr.sin_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr;

	ping_info.dns_resolution.resolved_ip = ping_info.dns_resolution.dest_addr.sin_addr;

    freeaddrinfo(res);
}

