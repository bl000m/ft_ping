#include "ft_ping.h"

// Define and initialize the global info structure
struct info g_info = {
    .sockfd = 0,               // Initialize socket file descriptor to 0
    .transmitted = 0,          // Initialize transmitted packets count to 0
    .received = 0,             // Initialize received packets count to 0
    .min_time = 1e6,           // Initialize minimum round-trip time to a large value
    .max_time = 0,             // Initialize maximum round-trip time to 0
    .total_time = 0,           // Initialize total round-trip time to 0
    .total_time_squared = 0,   // Initialize sum of squares of round-trip times to 0
    .dest = {0}                // Initialize destination IP address to 0.0.0.0
};

int main(int argc, char *argv[]) {
    struct options opts = {0};
    parse_options(argc, argv, &opts);

    signal(SIGINT, handle_signal);

    struct sockaddr_in dest;
    struct packet pkt;
    struct timeval tv_send, tv_receive;

    resolve_dns(g_info.hostname, &dest);
    print_start_message(g_info.hostname, &dest, &opts);
    setup_socket();
    prepare_packet(&pkt);

    while (1) {
        struct timeval loop_start, loop_end;
        gettimeofday(&loop_start, NULL);

        send_packet(&pkt, &dest, &tv_send);
        receive_and_print_packet(&pkt, &dest, &tv_receive, &tv_send);

        gettimeofday(&loop_end, NULL);
        long elapsed_time_us = (loop_end.tv_sec - loop_start.tv_sec) * 1000000 + (loop_end.tv_usec - loop_start.tv_usec);
        long remaining_time_us = 1000000 - elapsed_time_us;

        if (remaining_time_us > 0) {
            usleep(remaining_time_us);
        }
    }

    close(g_info.sockfd);
    return 0;
}

void handle_signal(int signo) {
    if (signo == SIGINT) {
        print_statistics();
        close(g_info.sockfd);
        exit(0);
    }
}

void send_packet(struct packet *pkt, struct sockaddr_in *dest, struct timeval *tv_send) {
    pkt->header.un.echo.sequence++;
    pkt->header.checksum = 0;
    pkt->header.checksum = calculate_checksum(pkt, sizeof(struct packet));

    gettimeofday(tv_send, NULL);

    if (sendto(g_info.sockfd, pkt, sizeof(struct packet), 0, (struct sockaddr*)dest, sizeof(struct sockaddr_in)) <= 0) {
        exit_with_error("sendto");
    }
    g_info.transmitted++;
}

void receive_and_print_packet(struct packet *pkt, struct sockaddr_in *r_addr, struct timeval *tv_receive, struct timeval *tv_send) {
    socklen_t len = sizeof(struct sockaddr_in);
    char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE];

    struct msghdr msg = {0};
    struct iovec iov[1] = {0};

    iov[0].iov_base = recv_buffer;
    iov[0].iov_len = sizeof(recv_buffer);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = r_addr;
    msg.msg_namelen = len;

    ssize_t bytes_received = recvmsg(g_info.sockfd, &msg, 0);
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        } else {
            exit_with_error("recvmsg");
        }
    }

    if (r_addr->sin_addr.s_addr != g_info.dest.s_addr) {
        return;
    }

    gettimeofday(tv_receive, NULL);
    struct ip *ip_header = (struct ip *)recv_buffer;
    int ttl = ip_header->ip_ttl;

    g_info.received++;
    double rtt = ((tv_receive->tv_sec - tv_send->tv_sec) * 1000000 + (tv_receive->tv_usec - tv_send->tv_usec)) / 1000.0;
    g_info.total_time += rtt;
    if (rtt < g_info.min_time) g_info.min_time = rtt;
    if (rtt > g_info.max_time) g_info.max_time = rtt;
    g_info.total_time_squared += rtt * rtt;

    printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
           bytes_received - IP_HEADER_SIZE,
           inet_ntoa(r_addr->sin_addr),
           pkt->header.un.echo.sequence,
           ttl,
           rtt);
}

void exit_with_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
