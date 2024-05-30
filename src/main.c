#include "ft_ping.h"

ft_ping_t ping_info; 
// Global variables are allocated memory at program startup and deallocated when the program terminates.

int main(int argc, char *argv[]) {

    init_info();
    signal(SIGINT, handling_sig);
    parsing_arguments(argc, argv);
    resolve_hostname();
    print_info();
    create_icmp_socket();
    set_socket_options();
    initialize_icmp_packet();
    fd_set read_fds;
    int max_fd = ping_info.sockfd + 1;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(ping_info.sockfd, &read_fds);

        // Set timeout for select (1 second)
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ready = select(max_fd, &read_fds, NULL, NULL, &timeout);
        if (ready == -1) {
            if (errno != EINTR) {
                perror("select");
                break; // Exit loop on error (excluding interrupt)
            }
        } else if (ready > 0) {
            // Data available to read from the socket
            if (FD_ISSET(ping_info.sockfd, &read_fds)) {
                // Receive ICMP reply
                receive_icmp_reply();
            }
        }

        // Send ICMP request regardless of whether there's data to read or not
        send_icmp_request();
    }

    // Main loop to send and receive ICMP packets
    // while (1) {
        // struct timeval loop_start, loop_end;
        // gettimeofday(&loop_start, NULL);

        // Send ICMP request
        // send_icmp_request();
        
        // Receive ICMP reply
        // receive_icmp_reply();

        // gettimeofday(&loop_end, NULL);
        // long elapsed_time_us = (loop_end.tv_sec - loop_start.tv_sec) * 1000000 + (loop_end.tv_usec - loop_start.tv_usec);
        // long remaining_time_us = 1000000 - elapsed_time_us;

        // if (remaining_time_us > 0) {
        //     usleep(remaining_time_us);
        // }
    // }

    close(ping_info.sockfd);
    return 0;
}

