#include "ft_ping.h"

// Function to create a raw socket for ICMP communication
void create_icmp_socket(socket_mgmt_t *socket_mgmt) {
    // Create a raw socket with protocol set to ICMP
    socket_mgmt->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_mgmt->sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set the Time-To-Live (TTL) value for the socket
    socket_mgmt->ttl = 64;
    if (setsockopt(socket_mgmt->sockfd, IPPROTO_IP, IP_TTL, &(socket_mgmt->ttl), sizeof(socket_mgmt->ttl)) != 0) {
        perror("setsockopt");
        close(socket_mgmt->sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Created ICMP socket with file descriptor: %d\n", socket_mgmt->sockfd);
}