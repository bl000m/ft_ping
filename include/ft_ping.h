#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#define PACKET_SIZE 64
#define IP_HEADER_SIZE 20

struct options {
    int count;
};

struct packet {
    struct icmphdr header;
    char msg[PACKET_SIZE - sizeof(struct icmphdr)];
};

struct info {
    int sockfd;               // File descriptor for the socket
    int transmitted;          // Number of packets transmitted
    int received;             // Number of packets received
    double min_time;          // Minimum round-trip time
    double max_time;          // Maximum round-trip time
    double total_time;        // Total round-trip time
    double total_time_squared; // Sum of squares of round-trip times (for standard deviation calculation)
    char *hostname;
    struct in_addr dest;      // Destination IP address
};

extern struct info g_info;

void parsing_arguments(int argc, char *argv[]);
void print_usage();

#endif
