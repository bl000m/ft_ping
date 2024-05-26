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
    printf("        ...not at the moment.\n\n");
    printf(COLOR_GREEN"Options valid for all request types:\n\n"COLOR_RESET);
    printf("-v             verbose output\n\n");
    printf(COLOR_GREEN"Options valid for echo requests:\n\n"COLOR_RESET);
    printf("-?                 give this help list\n\n");
    // printf("-V, --version              print program version\n\n");
    printf("Report bugs to <bug-mpagani@gnu.org>.");
}

void print_statistics(stats_t *stats, int packets_sent) {
    double avg_rtt;

    // Calculate average RTT
    avg_rtt = stats->total_rtt / stats->packets_received;

    // Print statistics
    printf("Packets sent: %d\n", packets_sent);
    printf("Packets received: %d\n", stats->packets_received);
    printf("Minimum RTT: %.2f ms\n", stats->min_rtt);
    printf("Maximum RTT: %.2f ms\n", stats->max_rtt);
    printf("Average RTT: %.2f ms\n", avg_rtt);
}