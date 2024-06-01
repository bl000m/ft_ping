#include "ft_ping.h"

void handling_error(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    close(ping_info.sockfd);
    exit(1);
}

void handling_gai_error(const char *msg, int error_code) {
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(error_code));
    close(ping_info.sockfd);
    exit(1);
}


void calculate_rtt(struct timeval *send_time, struct timeval *recv_time, stats_t *stats) {
    double rtt = ((recv_time->tv_sec - send_time->tv_sec) * 1000.0) +
                 ((recv_time->tv_usec - send_time->tv_usec) / 1000.0);
    stats->total_rtt += rtt;
    if (rtt < stats->min_rtt) stats->min_rtt = rtt;
    if (rtt > stats->max_rtt) stats->max_rtt = rtt;
    stats->total_rtt_squared += rtt * rtt;
    // todo
    // stats->packets_received++;
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
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
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
