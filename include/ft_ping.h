#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // INET_ADDSTRLEN in resolve hostname
#include <netinet/ip_icmp.h>
#include <netdb.h> //addrinfo struct + getaddrinfo
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <getopt.h>
#include <stdbool.h>

#define PACKET_SIZE 64
#define IP_HEADER_SIZE 20

// Define color escape sequences
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RESET "\033[0m"

// Command-line Arguments Parsing: Variables to store options and arguments
typedef struct {
    bool verbose;
    bool show_help;
    char *hostname;
} cmd_args_t;

// DNS Resolution: Variables to store target IP address or hostname
typedef struct {
    struct sockaddr_in dest_addr;
    char *resolved_ip;
} dns_resolution_t;

// Socket Management: Variables for socket descriptors and related settings
typedef struct {
    int sockfd;
    int ttl;
} socket_mgmt_t;

// ICMP Packet Handling: Variables for packet construction, sequence numbers, and IDs
typedef struct {
    struct icmp icmp_pkt;
    int seq_num;
    pid_t pid;
    unsigned short checksum;
} icmp_packet_t;

typedef struct {
    int packets_sent;
    int packets_received;
    double min_rtt;
    double max_rtt;
    double total_rtt;
} stats_t;

// Main program context structure to encompass all domains
typedef struct {
    cmd_args_t cmd_args;
    dns_resolution_t dns_resolution;
    socket_mgmt_t socket_mgmt;
    icmp_packet_t icmp_packet;
    stats_t stats;
    struct timeval start_time;
} ft_ping_t;

// extern struct info g_info;

void parsing_arguments(int argc, char *argv[], cmd_args_t *cmd_args);
void print_unrecognized_option(const char *option);
void print_no_args();
void print_help();
void resolve_hostname(const char *hostname, dns_resolution_t *dns_resolution);
void create_icmp_socket(socket_mgmt_t *socket_mgmt);
void set_socket_options(socket_mgmt_t *socket_mgmt);
void send_icmp_request(socket_mgmt_t *socket_mgmt, icmp_packet_t *icmp_packet, dns_resolution_t *dns_resolution);
void receive_icmp_reply(socket_mgmt_t *socket_mgmt, icmp_packet_t *icmp_packet, stats_t *stats);
void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats);
void print_statistics(stats_t *stats, int packets_sent);
unsigned short calculate_checksum(unsigned short *paddress, int len);
void handle_errors(void);
void cleanup_resources(socket_mgmt_t *socket_mgmt);

#endif
