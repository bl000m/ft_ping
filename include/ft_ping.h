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

void parse_options(int argc, char *argv[], struct options *opts);
void resolve_dns(const char *hostname, struct sockaddr_in *dest);
void print_start_message(const char *hostname, const struct sockaddr_in *dest, const struct options *opts);
void setup_socket(void);
void prepare_packet(struct packet *pkt);
void send_packet(struct packet *pkt, struct sockaddr_in *dest, struct timeval *tv_send);
void receive_and_print_packet(struct packet *pkt, struct sockaddr_in *r_addr, struct timeval *tv_receive, struct timeval *tv_send);
void print_statistics(void);
void handle_signal(int signo);
void exit_with_error(const char *message);
unsigned short calculate_checksum(void *b, int len);

#endif
