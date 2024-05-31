#include "ft_ping.h"

void print_no_args() {
    printf("./ft_ping: missing host operand\n");
    printf("Try 'ping -?' for more information.\n");
}

void print_unrecognized_option(const char *option) {
    printf("./ft_ping: unrecognized option '%s'\n", option);
    printf("Try 'ping --help' or 'ping --usage' for more information.\n");
}

void print_help() {
    printf("Usage: ./ft_ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
    printf(COLOR_GREEN "Options controlling ICMP request types:\n\n" COLOR_RESET);
    printf("...     not at the moment\n\n");
    printf(COLOR_GREEN"Options valid for all request types:\n\n"COLOR_RESET);
    printf("-v      verbose output\n\n");
    printf(COLOR_GREEN"Options valid for echo requests:\n\n"COLOR_RESET);
    printf("-?      give this help list\n\n");
    // printf("-V, --version              print program version\n\n");
    printf("Report bugs to <bug-mpagani@gnu.org>.");
}


void print_icmp_packet_info(ssize_t bytes_received, char *dest_addr, uint16_t sequence, uint8_t ttl, double rtt) {
    printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
           bytes_received,
           dest_addr,
           sequence,
           ttl,
           round(rtt * 1000) / 1000); 
}


void	print_statistics(void) {
    printf("\n--- %s statistics ---\n", ping_info.cmd_args.hostname);
    float ratio = (ping_info.stats.packets_sent - ping_info.stats.packets_received) / ping_info.stats.packets_sent;
    printf("%u packets transmitted, %u packets received, ", ping_info.stats.packets_sent, ping_info.stats.packets_received);
    
    if (ratio == 0) {
        printf("0%% packet loss\n");
    } else {
        printf("%.1f%% packet loss\n", 100.0 * ratio);
    }
    if (ping_info.stats.packets_received > 0) {
        double avg_time = ping_info.stats.total_rtt / ping_info.stats.packets_received;
        double stddev = sqrt((ping_info.stats.total_rtt_squared / ping_info.stats.packets_received) - (avg_time * avg_time));

        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping_info.stats.min_rtt, avg_time, ping_info.stats.max_rtt, stddev);
    }
}

void print_info() {
    int pid = getpid();
    if (ping_info.cmd_args.verbose) {
        printf("FT_PING %s (%s): %d data bytes, id 0x%x = %d\n", ping_info.cmd_args.hostname, inet_ntoa(ping_info.dns_resolution.dest_addr.sin_addr), PAYLOAD_SIZE, pid, pid);
    }
    else {
        printf("FT_PING %s (%s): %d data bytes\n", ping_info.cmd_args.hostname, inet_ntoa(ping_info.dns_resolution.dest_addr.sin_addr), PAYLOAD_SIZE);
    }
}