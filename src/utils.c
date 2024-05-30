#include "ft_ping.h"

void exit_with_error(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    close(ping_info.sockfd);
    exit(1);
}

void exit_with_gai_error(const char *msg, int error_code) {
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(error_code));
    close(ping_info.sockfd);
    exit(1);
}