#include "ft_ping.h"

// Signal handler function to catch SIGINT
void handling_sig(int sig) {
    if (sig == SIGINT){
        print_statistics(&ping_info.stats, ping_info.stats.packets_sent);
        close(ping_info.socket_mgmt.sockfd);
        exit(0);
	}
	// if (sig == SIGALRM)
	// 	g_params->signals.send = 1;
}