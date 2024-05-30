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

    send_receive_icmp_packets();
    close(ping_info.sockfd);
    return 0;
}