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


    if (ping_info.stats.packets_sent != 0) {
        ping_info.icmp_packet.header.un.echo.sequence++;
    }
    ping_info.icmp_packet.header.checksum = 0;
    ping_info.icmp_packet.header.checksum = calculate_checksum(&ping_info.icmp_packet, sizeof(sizeof(icmp_packet_t)));

    gettimeofday(&(ping_info.send_time), NULL);

    int sent_bytes = sendto(ping_info.sockfd, &ping_info.icmp_packet, sizeof(icmp_packet_t), 0, 
        (struct sockaddr *)&(ping_info.dns_resolution.dest_addr), sizeof(struct sockaddr_in));
    if (sent_bytes <= 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    ping_info.stats.packets_sent++;
}


void receive_icmp_reply() {
    char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE];
    struct msghdr message;
    struct iovec io_vector[1];

    initialize_message_buffers(recv_buffer, &message, io_vector);

    ssize_t bytes_received = receive_icmp_packet(&message);
    if (bytes_received <= 0) {
        return;
    }

    if (validate_received_packet(bytes_received) < 0) {
        return;
    }

    struct timeval receive_time;
    gettimeofday(&receive_time, NULL);

    calculate_rtt(&ping_info.send_time, &receive_time, &ping_info.stats);

    process_icmp_reply(recv_buffer, bytes_received, &receive_time);
}


void initialize_message_buffers(char *recv_buffer, struct msghdr *message, struct iovec *io_vector) {
    memset(recv_buffer, 0, PACKET_SIZE + IP_HEADER_SIZE);
    memset(message, 0, sizeof(*message));
    memset(io_vector, 0, sizeof(*io_vector));

    io_vector[0].iov_base = recv_buffer;
    io_vector[0].iov_len = PACKET_SIZE + IP_HEADER_SIZE;
    message->msg_iov = io_vector;
    message->msg_iovlen = 1;
    message->msg_name = &ping_info.dns_resolution.dest_addr;
    message->msg_namelen = sizeof(struct sockaddr_in);
}


ssize_t receive_icmp_packet(struct msghdr *message) {
    ssize_t bytes_received = recvmsg(ping_info.sockfd, message, 0);
    if (bytes_received < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            handling_error("recvmsg");
        }
    }
    return bytes_received;
}


int validate_received_packet(ssize_t bytes_received) {
    if (bytes_received < (ssize_t)sizeof(struct icmphdr)) {
        printf("Received packet is too small\n");
        return -1;
    }
    return 0;
}


void process_icmp_reply(char *recv_buffer, ssize_t bytes_received, struct timeval *receive_time) {
    // Extracting IP header and ICMP header from recv_buffer
    struct ip *ip_header = (struct ip *)recv_buffer;
    struct icmphdr *icmp_reply = (struct icmphdr *)(recv_buffer + (ip_header->ip_hl << 2));

    ssize_t payload_bytes_received;
    char *dest_addr_str;
    uint16_t sequence_num;

    // Check if the received packet is an ICMP echo reply and matches the expected sequence ID
    if (icmp_reply->type == ICMP_ECHOREPLY && icmp_reply->un.echo.id == ping_info.icmp_packet.header.un.echo.id) {

        double rtt = ((receive_time->tv_sec - ping_info.send_time.tv_sec) * 1000.0) +
                     ((receive_time->tv_usec - ping_info.send_time.tv_usec) / 1000.0);


        payload_bytes_received = bytes_received - (ip_header->ip_hl << 2);
        dest_addr_str = inet_ntoa(ping_info.dns_resolution.dest_addr.sin_addr);
        sequence_num = icmp_reply->un.echo.sequence;
        print_icmp_packet_info(payload_bytes_received, dest_addr_str, sequence_num, ip_header->ip_ttl, rtt);
    } else {
        printf("Received ICMP packet of type %d\n", icmp_reply->type);
    }
}


