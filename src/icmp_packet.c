#include "ft_ping.h"

void initialize_icmp_packet(icmp_packet_t *icmp_packet) {
    printf("Initializing ICMP packet...\n");

    memset(icmp_packet, 0, sizeof(icmp_packet_t)); // Clear the packet

    icmp_packet->pid = getpid(); // Set the PID to the current process ID
    icmp_packet->seq_num = 0; // Initialize the sequence number to 0

    // Initialize the ICMP header
    icmp_packet->header.type = ICMP_ECHO; // Echo request
    icmp_packet->header.code = 0; // Must be 0 for Echo request
    icmp_packet->header.un.echo.id = htons(icmp_packet->pid);
    icmp_packet->header.un.echo.sequence = htons(icmp_packet->seq_num);
    icmp_packet->header.checksum = 0; // Checksum initially set to 0

   // Example: Fill the payload with the current time and some pattern
    printf("Filling payload with timestamp and 'A' characters...\n");
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    memcpy(icmp_packet->payload, &ts, sizeof(ts)); // Include a timestamp
    memset(icmp_packet->payload + sizeof(ts), 'A', PAYLOAD_SIZE - sizeof(ts)); // Fill the rest with 'A'
    printf("Payload filled.\n");

}

void finalize_icmp_packet(icmp_packet_t *icmp_packet) {
    printf("Finalizing ICMP packet...\n");

    icmp_packet->header.checksum = 0; // Ensure checksum is 0 before calculation
    printf("Calculating checksum...\n");
    icmp_packet->header.checksum = calculate_checksum(icmp_packet, sizeof(struct icmphdr) + PAYLOAD_SIZE);
    printf("Checksum calculated: %x\n", icmp_packet->header.checksum);

}

void send_icmp_request(socket_mgmt_t *socket_mgmt, icmp_packet_t *icmp_packet, dns_resolution_t *dns_resolution) {
    // Calculate the checksum before sending the ICMP packet
    icmp_packet->header.checksum = 0; // Ensure checksum is 0 before calculation
    printf("Calculating checksum...\n");
    icmp_packet->header.checksum = calculate_checksum(icmp_packet, sizeof(struct icmphdr) + PAYLOAD_SIZE);
    printf("Checksum calculated: %x\n", icmp_packet->header.checksum);

    // Set the send time before sending the ICMP packet
    gettimeofday(&(icmp_packet->send_time), NULL);

    // Send the ICMP packet
    if (sendto(socket_mgmt->sockfd, icmp_packet, sizeof(icmp_packet_t), 0, 
               (struct sockaddr *)&(dns_resolution->dest_addr), sizeof(struct sockaddr_in)) <= 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
}


void receive_icmp_reply(socket_mgmt_t *socket_mgmt, icmp_packet_t *icmp_packet, stats_t *stats) {
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE]; // Ensure buffer is large enough to include IP header
    struct timeval recv_time;

    // Receive the ICMP reply packet
    int bytes_received = recvfrom(socket_mgmt->sockfd, recv_buffer, sizeof(recv_buffer), 0,
                                  (struct sockaddr *)&sender_addr, &sender_addr_len);
    if (bytes_received < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    // Get current time for calculating RTT
    gettimeofday(&recv_time, NULL);
    
    // Calculate RTT and update statistics
    calculate_rtt(&(icmp_packet->send_time), &recv_time, stats);

    // Process the received packet
    struct ip *ip_header = (struct ip *)recv_buffer;
    struct icmp *icmp_reply = (struct icmp *)(recv_buffer + (ip_header->ip_hl << 2));

    if (icmp_reply->icmp_type == ICMP_ECHOREPLY && icmp_reply->icmp_id == icmp_packet->header.un.echo.id) {
        stats->packets_received++;
        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
               bytes_received - (ip_header->ip_hl << 2),
               inet_ntoa(sender_addr.sin_addr),
               ntohs(icmp_reply->icmp_seq),
               ip_header->ip_ttl,
               stats->total_rtt);
    }
}



void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats) {
    double rtt; // Round-trip time

    // Calculate RTT in milliseconds
    rtt = (recv_time->tv_sec - send_time->tv_sec) * 1000.0; // Seconds to milliseconds
    rtt += (recv_time->tv_usec - send_time->tv_usec) / 1000.0; // Microseconds to milliseconds

    // Update statistics
    if (stats->packets_received == 1) {
        // First received packet, initialize min and max RTT
        stats->min_rtt = rtt;
        stats->max_rtt = rtt;
    } else {
        // Update min and max RTT if necessary
        if (rtt < stats->min_rtt) {
            stats->min_rtt = rtt;
        }
        if (rtt > stats->max_rtt) {
            stats->max_rtt = rtt;
        }
    }

    // Update total RTT
    stats->total_rtt += rtt;
}

unsigned short calculate_checksum(void *buf, int len) {
    unsigned short *buffer = buf;
    unsigned long sum = 0;
    for (; len > 1; len -= 2) {
        sum += *buffer++;
    }
    if (len == 1) {
        sum += *(unsigned char*)buffer;
    }
    //  for ICMP the byte order for checksum calculation is standardized to be in big-endian format.
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}