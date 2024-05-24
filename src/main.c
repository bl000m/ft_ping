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
    parsing_arguments(argc, argv);
    // Initialize Variables

    // Perform DNS Resolution (Optional): 

    // Create ICMP Socket
    // (Create a raw socket for sending and receiving ICMP packets.)
    
    // Set TTL Value (Optional): Set the Time-To-Live (TTL) value 
    // for the outgoing ICMP packets. 
    // This can be done using the setsockopt function 
    // with the IP_TTL option.

    // Send ICMP Echo Requests: Send ICMP echo request packets to the target host in a loop. 
    // need to handle timeouts and receive ICMP echo reply packets.
    
    // Process ICMP Echo Replies: Receive ICMP echo reply packets and calculate round-trip times (RTT). Optionally, perform 
    // reverse DNS resolution on the IP addresses of the replying hosts.

    // Display Results: Format and display the results, including the number of packets transmitted, received, lost, and any relevant 
    // statistics such as minimum, maximum, and average RTT.

    // Handle Errors: Handle any errors that occur during the execution of the program, such as network errors, socket errors, or invalid command-line arguments.

    // Cleanup Resources: Close the ICMP socket and perform any necessary cleanup before exiting the program.

    return 0;
}

// void handle_signal(int signo) {

// }

// void send_packet(struct packet *pkt, struct sockaddr_in *dest, struct timeval *tv_send) {
//     pkt->header.un.echo.sequence++;
//     pkt->header.checksum = 0;
//     pkt->header.checksum = calculate_checksum(pkt, sizeof(struct packet));

//     gettimeofday(tv_send, NULL);

//     if (sendto(g_info.sockfd, pkt, sizeof(struct packet), 0, (struct sockaddr*)dest, sizeof(struct sockaddr_in)) <= 0) {
//         exit_with_error("sendto");
//     }
//     g_info.transmitted++;
// }

// void receive_and_print_packet(struct packet *pkt, struct sockaddr_in *r_addr, struct timeval *tv_receive, struct timeval *tv_send) {
//     socklen_t len = sizeof(struct sockaddr_in);
//     char recv_buffer[PACKET_SIZE + IP_HEADER_SIZE];

//     struct msghdr msg = {0};
//     struct iovec iov[1] = {0};

//     iov[0].iov_base = recv_buffer;
//     iov[0].iov_len = sizeof(recv_buffer);
//     msg.msg_iov = iov;
//     msg.msg_iovlen = 1;
//     msg.msg_name = r_addr;
//     msg.msg_namelen = len;

//     ssize_t bytes_received = recvmsg(g_info.sockfd, &msg, 0);
//     if (bytes_received < 0) {
//         if (errno == EAGAIN || errno == EWOULDBLOCK) {
//             return;
//         } else {
//             exit_with_error("recvmsg");
//         }
//     }

//     if (r_addr->sin_addr.s_addr != g_info.dest.s_addr) {
//         return;
//     }

//     gettimeofday(tv_receive, NULL);
//     struct ip *ip_header = (struct ip *)recv_buffer;
//     int ttl = ip_header->ip_ttl;

//     g_info.received++;
//     double rtt = ((tv_receive->tv_sec - tv_send->tv_sec) * 1000000 + (tv_receive->tv_usec - tv_send->tv_usec)) / 1000.0;
//     g_info.total_time += rtt;
//     if (rtt < g_info.min_time) g_info.min_time = rtt;
//     if (rtt > g_info.max_time) g_info.max_time = rtt;
//     g_info.total_time_squared += rtt * rtt;

//     printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
//            bytes_received - IP_HEADER_SIZE,
//            inet_ntoa(r_addr->sin_addr),
//            pkt->header.un.echo.sequence,
//            ttl,
//            rtt);
// }

// void exit_with_error(const char *message) {
//     perror(message);
//     exit(EXIT_FAILURE);
// }
