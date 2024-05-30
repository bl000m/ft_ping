#include "ft_ping.h"

 void init_info(void) {
   // Zero out the structure
    memset(&ping_info, 0, sizeof(ft_ping_t));

    // Initialize command-line arguments structure
    ping_info.cmd_args.verbose = false;
    ping_info.cmd_args.show_help = false;
    ping_info.cmd_args.hostname = NULL;

    // Initialize DNS resolution structure
    memset(&(ping_info.dns_resolution.dest_addr), 0, sizeof(struct sockaddr_in));
    ping_info.dns_resolution.resolved_ip = NULL;

    // Initialize socket management structure
    ping_info.socket_mgmt.sockfd = -1;  // Invalid socket descriptor
    ping_info.socket_mgmt.ttl = 64;     // Default TTL value

    // Initialize ICMP packet structure
    memset(&(ping_info.icmp_packet.header), 0, sizeof(struct icmphdr));
    memset(ping_info.icmp_packet.payload, 0, PACKET_SIZE - sizeof(struct icmphdr));
    ping_info.icmp_packet.seq_num = 0;
    ping_info.icmp_packet.pid = getpid();  // Set the process ID
    ping_info.icmp_packet.checksum = 0;
    memset(&(ping_info.icmp_packet.send_time), 0, sizeof(struct timeval));
    memset(&(ping_info.icmp_packet.received_time), 0, sizeof(struct timeval));

    // Initialize statistics structure
    ping_info.stats.packets_sent = 0;
    ping_info.stats.packets_received = 0;
    ping_info.stats.min_rtt = 1e6;
    ping_info.stats.max_rtt = 0.0;
    ping_info.stats.total_rtt = 0.0;
}