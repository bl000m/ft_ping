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