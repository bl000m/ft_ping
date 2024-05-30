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

    if (ping_info.cmd_args.verbose) {
        printf("Verbose mode enabled\n");
    }

    printf("Destination: %s\n", ping_info.cmd_args.hostname);
}

void resolve_hostname() {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    int error = getaddrinfo(ping_info.cmd_args.hostname, NULL, &hints, &res);
    if (error != 0) {
        exit_with_gai_error("getaddrinfo", error);
    }

    if (res == NULL) {
        printf("Host not found.\n");
        exit(1);
    }

    ping_info.dns_resolution.dest_addr.sin_family = AF_INET;
    ping_info.dns_resolution.dest_addr.sin_port = 0;
    ping_info.dns_resolution.dest_addr.sin_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr;

	ping_info.dns_resolution.resolved_ip = ping_info.dns_resolution.dest_addr.sin_addr;

    freeaddrinfo(res);
    printf("Resolved %s to IP address: %s\n", ping_info.cmd_args.hostname, inet_ntoa(ping_info.dns_resolution.resolved_ip));
}



// void resolve_hostname() {
//     struct addrinfo hints, *res, *p;
//     int status;

//     // Prepare the hints structure
//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_INET;  // Set hints.ai_family to AF_INET to indicate IPv4 addresses
//     hints.ai_socktype = SOCK_RAW;  // We need raw socket

//     // Get address information
//     if ((status = getaddrinfo(ping_info.cmd_args.hostname, NULL, &hints, &res)) != 0) {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
//         exit(EXIT_FAILURE);
//     }

//     // Loop through the results and pick the first one we can use
//     for (p = res; p != NULL; p = p->ai_next) {
//         struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;

//         // Store the destination address directly
//         ping_info.dns_resolution.dest_addr = *ipv4;

//         // Optional: Store the resolved IP address as a string for debugging/logging
//         inet_ntop(p->ai_family, &(ipv4->sin_addr), ping_info.dns_resolution.resolved_ip_str, sizeof(ping_info.dns_resolution.resolved_ip_str));

//         break;  // We only need the first valid result
//     }

//     // Free the linked list
//     freeaddrinfo(res);

//     // Check if we failed to resolve the ping_info.cmd_args.hostname
//     if (ping_info.dns_resolution.dest_addr.sin_addr.s_addr == 0) {
//         fprintf(stderr, "Failed to resolve ping_info.cmd_args.hostname: %s\n", ping_info.cmd_args.hostname);
//         exit(EXIT_FAILURE);
//     }

//     printf("Resolved ping_info.cmd_args.hostname %s to IP address: %s\n",
//            ping_info.cmd_args.hostname, ping_info.dns_resolution.resolved_ip_str);
// }