#include "ft_ping.h"

 void init_info(void) {
    memset(&ping_info, 0, sizeof(ft_ping_t));

    // command-line arguments 
    ping_info.cmd_args.verbose = false;
    ping_info.cmd_args.show_help = false;
    ping_info.cmd_args.hostname = NULL;

    // DNS resolution 
    memset(&(ping_info.dns_resolution.dest_addr), 0, sizeof(struct sockaddr_in));
    ping_info.dns_resolution.resolved_ip.s_addr = 0;

    // socket management 
    ping_info.sockfd = -1; 

    // ICMP packet 
    memset(&(ping_info.icmp_packet.header), 0, sizeof(struct icmphdr));
    memset(ping_info.icmp_packet.payload, 0, PACKET_SIZE - sizeof(struct icmphdr));
    memset(&(ping_info.send_time), 0, sizeof(struct timeval));
    memset(&(ping_info.received_time), 0, sizeof(struct timeval));

    ping_info.stats.packets_sent = 0;
    ping_info.stats.packets_received = 0;
    ping_info.stats.min_rtt = 1e6;
    ping_info.stats.max_rtt = 0.0;
    ping_info.stats.total_rtt = 0.0;
}

void initialize_icmp_packet() {

    memset(&ping_info.icmp_packet, 0, sizeof(icmp_packet_t));
    ping_info.icmp_packet.header.type = ICMP_ECHO; // Echo request
    ping_info.icmp_packet.header.code = 0; // Must be 0 for Echo request
    ping_info.icmp_packet.header.un.echo.id = getpid() & 0xFFFF;
    ping_info.icmp_packet.header.checksum = 0; // Checksum initially set to 0
    ping_info.icmp_packet.header.un.echo.sequence = 0; // Initialize sequence number to 0
}
