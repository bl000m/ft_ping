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
#include <math.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define PACKET_SIZE 64
#define IP_HEADER_SIZE 20
#define PAYLOAD_SIZE 56 

#define COLOR_GREEN "\033[0;32m"
#define COLOR_RESET "\033[0m"

typedef struct {
    bool verbose;
    bool show_help;
    char *hostname;
} cmd_args_t;

typedef struct {
    struct sockaddr_in dest_addr;
    struct in_addr resolved_ip;
} dns_resolution_t;


typedef struct {
    struct icmphdr header;
    char payload[PACKET_SIZE - sizeof(struct icmphdr)]; 
} icmp_packet_t;

typedef struct {
    int packets_sent;
    int packets_received;
    double min_rtt;
    double max_rtt;
    double total_rtt;
    double total_rtt_squared;
} stats_t;

typedef struct {
    cmd_args_t cmd_args;
    dns_resolution_t dns_resolution;
    icmp_packet_t icmp_packet;
    stats_t stats;
    int sockfd;
    struct timeval send_time;
    struct timeval received_time;
} ft_ping_t;

extern ft_ping_t ping_info;

// Initialization Functions
void init_info(void);

// Argument Parsing Functions
void parsing_arguments(int argc, char *argv[]);
void resolve_hostname(char *argv[]);

// Printing Functions
void print_unrecognized_option(const char *option);
void print_no_args();
void print_help();
void print_info();
void print_statistics();
void print_icmp_packet_info(ssize_t bytes_received, char *dest_addr, uint16_t sequence, uint8_t ttl, double rtt);


// Socket Management Functions
void create_icmp_socket();
void set_socket_options();
void set_socket_timeout(int seconds, int microseconds);

// ICMP Packet Handling Functions
void send_receive_icmp_packets();
void initialize_icmp_packet();
void send_icmp_request();
void receive_icmp_reply();
ssize_t receive_icmp_packet(struct msghdr *message);
int validate_received_packet(ssize_t bytes_received);
void process_icmp_reply(char *recv_buffer, ssize_t bytes_received, struct timeval *receive_time);
void initialize_message_buffers(char *recv_buffer, struct msghdr *message, struct iovec *io_vector);

// Round-Trip Time Calculation Functions
void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats);
unsigned short calculate_checksum(void *buf, int len);

// Error Handling Functions
void handling_gai_error(const char *msg, int error_code);
void handling_error(const char *msg);
void handling_sig(int sig);

// utils
long calculate_elapsed_time(struct timeval start, struct timeval end);
void sleep_for_remaining_time(long elapsed_time_us);




#endif
