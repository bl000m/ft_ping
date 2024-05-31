#include "ft_ping.h"

void handling_sig(int sig) {
    if (sig == SIGINT){
        print_statistics(&ping_info.stats, ping_info.stats.packets_sent);
        close(ping_info.sockfd);
        exit(0);
	}
}