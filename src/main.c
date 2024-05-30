#include "ft_ping.h"

ft_ping_t ping_info; 
// Global variables are allocated memory at program startup and deallocated when the program terminates.

int main(int argc, char *argv[]) {

    init_info();
    signal(SIGINT, handling_sig);
    parsing_arguments(argc, argv);

    resolve_hostname();
    create_icmp_socket();
    set_socket_options();

    // Initialize ICMP Packet
    initialize_icmp_packet();
    finalize_icmp_packet();

    // Main loop to send and receive ICMP packets
    while (true) {
        struct timeval loop_start, loop_end;
        gettimeofday(&loop_start, NULL);

        // Send ICMP request
        send_icmp_request();
        
        // Receive ICMP reply
        receive_icmp_reply();

        gettimeofday(&loop_end, NULL);
        long elapsed_time_us = (loop_end.tv_sec - loop_start.tv_sec) * 1000000 + (loop_end.tv_usec - loop_start.tv_usec);
        long remaining_time_us = 1000000 - elapsed_time_us;

        if (remaining_time_us > 0) {
            usleep(remaining_time_us);
        }
    }

    // Display Results: Format and display the results, including the number of packets transmitted, received, lost, and any relevant 
    // statistics such as minimum, maximum, and average RTT.

    // Handle Errors: Handle any errors that occur during the execution of the program, such as network errors, socket errors, or invalid command-line arguments.
    // handle_errors();

    // Cleanup Resources: Close the ICMP socket and perform any necessary cleanup before exiting the program.
    // cleanup_resources(&ping_info.socket_mgmt);
    close(ping_info.socket_mgmt.sockfd);
    return 0;
}

