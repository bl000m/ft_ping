#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // INET_ADDSTRLEN in resolve hostname
#include <netinet/ip_icmp.h> // icmp struct
#include <netdb.h> //addrinfo struct + getaddrinfo
#include <unistd.h> // for getpid()
#include <signal.h>// to handle SIGINT signal (Ctrl+C)
#include <errno.h>
#include <sys/time.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>

#define PACKET_SIZE 64
#define IP_HEADER_SIZE 20
#define PAYLOAD_SIZE 56 

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


typedef struct {
    struct icmphdr header; // ICMP header
    char payload[PAYLOAD_SIZE]; // ICMP payload
    int seq_num; // Sequence number
    pid_t pid; // Process ID
    unsigned short checksum; // Checksum
    struct timeval send_time;
    struct timeval received_time;
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
} ft_ping_t;

// Declare the global variable
extern ft_ping_t ping_info;

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
void handle_errors(void);
void cleanup_resources(socket_mgmt_t *socket_mgmt);
void initialize_icmp_packet(icmp_packet_t *icmp_packet);
void finalize_icmp_packet(icmp_packet_t *icmp_packet);
unsigned short calculate_checksum(void *buf, int len);
void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats);
void handling_sig(int sig);
void set_socket_timeout(socket_mgmt_t *socket_mgmt, int seconds, int microseconds);



#endif
