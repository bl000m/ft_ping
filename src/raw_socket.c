#include "ft_ping.h"

// Function to create a raw socket for ICMP communication
void create_icmp_socket(socket_mgmt_t *socket_mgmt) {
    // Create a raw socket with protocol set to ICMP
    socket_mgmt->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_mgmt->sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Created ICMP socket with file descriptor: %d\n", socket_mgmt->sockfd);
}

void set_socket_options(socket_mgmt_t *socket_mgmt) {
    // Set the Time-To-Live (TTL) value for the socket
    // This determines how many hops (routers) a packet can pass through before being discarded
    socket_mgmt->ttl = 64;
    if (setsockopt(socket_mgmt->sockfd, IPPROTO_IP, IP_TTL, &(socket_mgmt->ttl), sizeof(socket_mgmt->ttl)) != 0) {
        perror("setsockopt");
        close(socket_mgmt->sockfd);
        exit(EXIT_FAILURE);
    }

    set_socket_timeout(&ping_info.socket_mgmt, 1, 0); // Set timeout to 1 second

    printf("Set socket options for file descriptor: %d\n", socket_mgmt->sockfd);
}

void set_socket_timeout(socket_mgmt_t *socket_mgmt, int seconds, int microseconds) {
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = microseconds;

    if (setsockopt(socket_mgmt->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0) {
        perror("setsockopt for timeout");
        exit(EXIT_FAILURE);
    }

    printf("Set receive timeout for file descriptor %d\n", socket_mgmt->sockfd);
}