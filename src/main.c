#include "ft_ping.h"

ft_ping_t ping_info; 
// Global variables are allocated memory at program startup and deallocated when the program terminates.


int main(int argc, char *argv[]) {
    init_info();
    initialize_icmp_packet();
    signal(SIGINT, handling_sig);
    parsing_arguments(argc, argv);
    resolve_hostname(argv);
    print_info();
    create_icmp_socket();
    set_socket_options();

    send_receive_icmp_packets();
    close(ping_info.sockfd);
    return 0;
}