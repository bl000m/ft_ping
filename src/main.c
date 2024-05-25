#include "ft_ping.h"

int main(int argc, char *argv[]) {
    // Initialize Variables
    ft_ping_t ping_info = {0}; // Initialize the context structure
    parsing_arguments(argc, argv, &ping_info.cmd_args);

    // Perform DNS Resolution (Optional): 
    resolve_hostname(ping_info.cmd_args.hostname, &ping_info.dns_resolution);

    // Create ICMP Socket
    // (Create a raw socket for sending and receiving ICMP packets.)
    // create_icmp_socket(&ping_info.socket_mgmt);

    // Set TTL Value (Optional): Set the Time-To-Live (TTL) value 
    // for the outgoing ICMP packets. 
    // This can be done using the setsockopt function 
    // with the IP_TTL option.
    // set_socket_options(&ping_info.socket_mgmt);

    // Initialize ICMP Packet
    // ping_info.icmp_packet.pid = getpid();
    // ping_info.icmp_packet.seq_num = 0;

    // Main loop to send and receive ICMP packets
    // while (true) {
    //     send_icmp_request(&ping_info.socket_mgmt, &ping_info.icmp_packet, &ping_info.dns_resolution);
    //     receive_icmp_reply(&ping_info.socket_mgmt, &ping_info.icmp_packet, &ping_info.stats);
        
    //     // Add a sleep to manage the interval between sends
    //     sleep(1);
    // }

    // Display Results: Format and display the results, including the number of packets transmitted, received, lost, and any relevant 
    // statistics such as minimum, maximum, and average RTT.
    // print_statistics(&ping_info.stats, ping_info.stats.packets_sent);

    // Handle Errors: Handle any errors that occur during the execution of the program, such as network errors, socket errors, or invalid command-line arguments.
    // handle_errors();

    // Cleanup Resources: Close the ICMP socket and perform any necessary cleanup before exiting the program.
    // cleanup_resources(&ping_info.socket_mgmt);

    return 0;
}

