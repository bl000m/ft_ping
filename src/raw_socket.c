#include "ft_ping.h"

// Function to create a raw socket for ICMP communication
void create_icmp_socket() {
    // Create a raw socket with protocol set to ICMP
    ping_info.socket_mgmt.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_info.socket_mgmt.sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // printf("Created ICMP socket with file descriptor: %d\n", ping_info.socket_mgmt.sockfd);
}

void set_socket_options() {
    // Set the Time-To-Live (TTL) value for the socket
    // This determines how many hops (routers) a packet can pass through before being discarded
    ping_info.socket_mgmt.ttl = 64;
    if (setsockopt(ping_info.socket_mgmt.sockfd, IPPROTO_IP, IP_TTL, &(ping_info.socket_mgmt.ttl), sizeof(int)) != 0) {
        perror("setsockopt");
        close(ping_info.socket_mgmt.sockfd);
        exit(EXIT_FAILURE);
    }

    set_socket_timeout(1, 0); // Set timeout to 1 second

}

void set_socket_timeout(int seconds, int microseconds) {
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = microseconds;

    if (setsockopt(ping_info.socket_mgmt.sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0) {
        perror("setsockopt for timeout");
        exit(EXIT_FAILURE);
    }

}