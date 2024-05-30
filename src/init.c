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
    ping_info.dns_resolution.resolved_ip.s_addr = 0;

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

void initialize_icmp_packet() {
    printf("Initializing ICMP packet...\n");

    // Clear the packet
    memset(&ping_info.icmp_packet, 0, sizeof(icmp_packet_t));

    // Initialize the ICMP header
    ping_info.icmp_packet.header.type = ICMP_ECHO; // Echo request
    ping_info.icmp_packet.header.code = 0; // Must be 0 for Echo request
    ping_info.icmp_packet.header.un.echo.id = htons(ping_info.icmp_packet.pid);
    ping_info.icmp_packet.header.un.echo.sequence = htons(ping_info.icmp_packet.seq_num);
    ping_info.icmp_packet.header.checksum = 0; // Checksum initially set to 0


}


void finalize_icmp_packet() {
    ping_info.icmp_packet.header.checksum = 0; // Ensure checksum is 0 before calculation
    ping_info.icmp_packet.header.checksum = calculate_checksum(&(ping_info.icmp_packet), sizeof(icmp_packet_t));
    printf("Checksum calculated: %x\n", ping_info.icmp_packet.header.checksum);

}