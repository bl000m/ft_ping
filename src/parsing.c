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

void resolve_hostname(const char *hostname, dns_resolution_t *dns_resolution) {
    struct addrinfo hints, *res, *p;
    int status;
    char resolved_ip_str[INET_ADDRSTRLEN];
    // The INET_ADDRSTRLEN macro is defined in <arpa/inet.h>. It specifies the length of the string buffer needed to hold the IPv4 address in string form, including the terminating null byte. Specifically, INET_ADDRSTRLEN is defined as 16, which is enough to hold the longest possible IPv4 address in dotted-decimal format (e.g., "255.255.255.255") plus the null terminator.

    // Prepare the hints structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // Set hints.ai_family to AF_INET to indicate IPv4 addresses
    hints.ai_socktype = SOCK_RAW;  // We need raw socket

    // Get address information
    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Loop through the results and pick the first one we can use
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);

        // Convert the IP address to a string and store it
        inet_ntop(p->ai_family, addr, resolved_ip_str, sizeof(resolved_ip_str));
        dns_resolution->resolved_ip = strdup(resolved_ip_str);  // Store resolved IP as a string
        dns_resolution->dest_addr = *ipv4;  // Store destination address

        break;  // We only need the first valid result
    }

    // Free the linked list
    freeaddrinfo(res);

    // Check if we failed to resolve the hostname
    if (!dns_resolution->resolved_ip) {
        fprintf(stderr, "Failed to resolve hostname: %s\n", hostname);
        exit(EXIT_FAILURE);
    }

    printf("Resolved hostname %s to IP address: %s\n", hostname, dns_resolution->resolved_ip);
}



