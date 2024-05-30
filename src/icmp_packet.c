#include "ft_ping.h"

void send_receive_icmp_packets() {
    while (1) {
        struct timeval loop_start, loop_end;
        gettimeofday(&loop_start, NULL);

        send_icmp_request();
        receive_icmp_reply();

        gettimeofday(&loop_end, NULL);
        long elapsed_time_us = calculate_elapsed_time(loop_start, loop_end);
        sleep_for_remaining_time(elapsed_time_us);
    }
}

long calculate_elapsed_time(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

void sleep_for_remaining_time(long elapsed_time_us) {
    long remaining_time_us = 1000000 - elapsed_time_us;
    if (remaining_time_us > 0) {
        usleep(remaining_time_us);
    }
}

void send_icmp_request() {
    // Increment sequence number
    ping_info.icmp_packet.header.un.echo.sequence++;

    // Calculate checksum before sending
    // extremely important that checksum is calculated on the whole icmp packet
    // because the receiver will make the checksum of the whole packet
    // if the checksum doesnt' match the receiver discard the packet
    // and the consequence is that the sender will no receive packets back
    ping_info.icmp_packet.header.checksum = 0;
    ping_info.icmp_packet.header.checksum = calculate_checksum(&ping_info.icmp_packet, sizeof(sizeof(icmp_packet_t)));

    // Set send time
    gettimeofday(&(ping_info.send_time), NULL);
    // printf("Send time: %ld seconds, %ld microseconds\n", ping_info.send_time.tv_sec, ping_info.send_time.tv_usec);

    // Send ICMP packet
    int sent_bytes = sendto(ping_info.sockfd, &ping_info.icmp_packet, sizeof(icmp_packet_t), 0, 
        (struct sockaddr *)&(ping_info.dns_resolution.dest_addr), sizeof(struct sockaddr_in));
    if (sent_bytes <= 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    // printf("Sent %d bytes\n", sent_bytes);

    ping_info.stats.packets_sent++;
}


void receive_icmp_reply() {
    socklen_t sender_addr_len = sizeof(struct sockaddr_in);
    // when we receive the packet bck it contains both the ip header and the icmp packet (that is encapsulated in the ip packet)
    // so the buffer size should be large enough for both
    // differnently from the sending process where the sendto func is in charge only of the icmp packet
    // this is because it's the OS network stack  in charge of incapsulating it within the IP pcket
    char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE];

    struct msghdr msg;
    struct iovec iov[1];

    memset(&msg, 0, sizeof(msg));
    memset(&iov, 0, sizeof(iov));

    iov[0].iov_base = recv_buffer;
    iov[0].iov_len = sizeof(recv_buffer);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = &ping_info.dns_resolution.dest_addr;
    msg.msg_namelen = sender_addr_len;


    ssize_t bytes_received = recvmsg(ping_info.sockfd, &msg, 0);
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return;
		}
		else {
			handling_error("recvmsg");
		}
    }


    // Ensure received data is large enough to contain ICMP header
    if (bytes_received < (ssize_t)sizeof(struct icmp)) {
        printf("Received packet is too small\n");
        return;
    }

    // Get current time for calculating RTT
    struct timeval received_time;
    gettimeofday(&received_time, NULL);

    // Calculate RTT and update statistics
    calculate_rtt(&(ping_info.send_time), &received_time, &ping_info.stats);

    // Process the received packet
    struct ip *ip_header = (struct ip *)recv_buffer;
    struct icmp *icmp_reply = (struct icmp *)(recv_buffer + (ip_header->ip_hl << 2));
    if (icmp_reply->icmp_type == ICMP_ECHOREPLY && icmp_reply->icmp_id == ping_info.icmp_packet.header.un.echo.id) {
        double rtt = ((received_time.tv_sec - ping_info.send_time.tv_sec) * 1000.0) +
                     ((received_time.tv_usec - ping_info.send_time.tv_usec) / 1000.0);
        printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
               bytes_received - (ip_header->ip_hl << 2),
               inet_ntoa(ping_info.dns_resolution.dest_addr.sin_addr),
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
    stats->total_rtt_squared += rtt * rtt;
    stats->packets_received++;
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