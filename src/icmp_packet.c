#include "ft_ping.h"

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

void send_icmp_request() {
    printf("Sending ICMP request...\n");

    // Increment sequence number
    ping_info.icmp_packet.header.un.echo.sequence++;

    // Calculate checksum before sending
    printf("Calculating checksum before sending...\n");
    ping_info.icmp_packet.header.checksum = 0;
    ping_info.icmp_packet.header.checksum = calculate_checksum(&(ping_info.icmp_packet), sizeof(icmp_packet_t));
    printf("Checksum calculated before sending: %x\n", ping_info.icmp_packet.header.checksum);

    // Set send time
    gettimeofday(&(ping_info.icmp_packet.send_time), NULL);
    printf("Send time: %ld seconds, %ld microseconds\n", ping_info.icmp_packet.send_time.tv_sec, ping_info.icmp_packet.send_time.tv_usec);

    // Send ICMP packet
    int sent_bytes = sendto(ping_info.socket_mgmt.sockfd, &ping_info.icmp_packet, sizeof(icmp_packet_t), 0, 
                            (struct sockaddr *)&(ping_info.dns_resolution.dest_addr), sizeof(struct sockaddr_in));
    if (sent_bytes <= 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("Sent %d bytes\n", sent_bytes);

    ping_info.stats.packets_sent++;
}

void receive_icmp_reply() {
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE]; // Ensure buffer is large enough to include IP header

    struct msghdr msg;
    struct iovec iov[1];

    memset(&msg, 0, sizeof(msg));
    memset(&iov, 0, sizeof(iov));

    iov[0].iov_base = recv_buffer;
    iov[0].iov_len = sizeof(recv_buffer);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = &sender_addr;
    msg.msg_namelen = sender_addr_len;

    // Receive the ICMP reply packet
    ssize_t bytes_received = recvmsg(ping_info.socket_mgmt.sockfd, &msg, 0);
    if (bytes_received < 0) {
        perror("recvmsg");
        return;
    }

    printf("Received %zd bytes from %s\n", bytes_received, inet_ntoa(sender_addr.sin_addr));

    // Get current time for calculating RTT
    struct timeval received_time;
    gettimeofday(&received_time, NULL);

    // Calculate RTT and update statistics
    calculate_rtt(&(ping_info.icmp_packet.send_time), &received_time, &ping_info.stats);

    // Process the received packet
    struct ip *ip_header = (struct ip *)recv_buffer;
    struct icmp *icmp_reply = (struct icmp *)(recv_buffer + (ip_header->ip_hl << 2));
    if (icmp_reply->icmp_type == ICMP_ECHOREPLY && icmp_reply->icmp_id == ping_info.icmp_packet.header.un.echo.id) {
        ping_info.stats.packets_received++;
        double rtt = ((received_time.tv_sec - ping_info.icmp_packet.send_time.tv_sec) * 1000.0) +
                     ((received_time.tv_usec - ping_info.icmp_packet.send_time.tv_usec) / 1000.0);
        printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
               bytes_received - (ip_header->ip_hl << 2),
               inet_ntoa(sender_addr.sin_addr),
               ntohs(icmp_reply->icmp_seq),
               ip_header->ip_ttl,
               rtt);
    } else {
        printf("Received ICMP packet of type %d\n", icmp_reply->icmp_type);
    }
}



void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats) {
    double rtt = ((recv_time->tv_sec - send_time->tv_sec) * 1000.0) +
                 ((recv_time->tv_usec - send_time->tv_usec) / 1000.0);
    stats->total_rtt += rtt;
    if (rtt < stats->min_rtt) stats->min_rtt = rtt;
    if (rtt > stats->max_rtt) stats->max_rtt = rtt;
    stats->packets_received++;
    printf("RTT: %.2f ms\n", rtt);
}

// void receive_icmp_reply(socket_mgmt_t *socket_mgmt, icmp_packet_t *icmp_packet, stats_t *stats) {
//     struct sockaddr_in sender_addr;
//     socklen_t sender_addr_len = sizeof(sender_addr);
//     char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE]; // Ensure buffer is large enough to include IP header

//     // Receive the ICMP reply packet
//     int bytes_received = recvfrom(ping_info.socket_mgmt.sockfd, recv_buffer, sizeof(recv_buffer), 0,
//                                   (struct sockaddr *)&sender_addr, &sender_addr_len);
//     if (bytes_received < 0) {
//         perror("recvfrom");
//         exit(EXIT_FAILURE);
//     }

//         printf("Received from: %s:%d\n", inet_ntoa(sender_addr.sin_addr), ntohs(sender_addr.sin_port));


//     // Get current time for calculating RTT
//     gettimeofday(&icmp_packet.received_time, NULL);
    
//     // Calculate RTT and update statistics
//     calculate_rtt(&(icmp_packet.send_time), &(icmp_packet.received_time), stats);

//     // Process the received packet
//     struct ip *ip_header = (struct ip *)recv_buffer;
//     struct icmp *icmp_reply = (struct icmp *)(recv_buffer + (ip_header->ip_hl << 2));

//     if (icmp_reply->icmp_type == ICMP_ECHOREPLY && icmp_reply->icmp_id == icmp_packet.header.un.echo.id) {
//         stats->packets_received++;
//         printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
//                bytes_received - (ip_header->ip_hl << 2),
//                inet_ntoa(sender_addr.sin_addr),
//                ntohs(icmp_reply->icmp_seq),
//                ip_header->ip_ttl,
//                stats->total_rtt);
//     }
// }



// void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats) {
//     double rtt; // Round-trip time

//     // Calculate RTT in milliseconds
//     rtt = (recv_time->tv_sec - send_time->tv_sec) * 1000.0; // Seconds to milliseconds
//     rtt += (recv_time->tv_usec - send_time->tv_usec) / 1000.0; // Microseconds to milliseconds

//     // Update statistics
//     if (stats->packets_received == 1) {
//         // First received packet, initialize min and max RTT
//         stats->min_rtt = rtt;
//         stats->max_rtt = rtt;
//     } else {
//         // Update min and max RTT if necessary
//         if (rtt < stats->min_rtt) {
//             stats->min_rtt = rtt;
//         }
//         if (rtt > stats->max_rtt) {
//             stats->max_rtt = rtt;
//         }
//     }

//     // Update total RTT
//     stats->total_rtt += rtt;
// }

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