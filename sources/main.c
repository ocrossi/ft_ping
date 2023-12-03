#include "../includes/ft_ping.h"
#include <signal.h>

char acceptedFlags[] = "v?ctW";
t_statData *globalStats;


// so apparently im forced to go global here
// https://stackoverflow.com/questions/6970224/providing-passing-argument-to-signal-handler

void print_stats(int signum) {
	printf("\nSTAAAAAAATS\n");
	exit(1);
}

int main(int ac, char** av) {
	t_pingData data;
	t_statData stats;
	int sockFd;
	char *rpack;

	data = parsing(ac, &stats, av);
	ft_memset(&stats, 0, sizeof(stats));
	globalStats = &stats;
	sockFd = create_packet(&data);
	signal(SIGINT, &print_stats);
	while (1) {
		if (data.max_ping != 0 && data.pingNb == data.max_ping) 
			break;
		data.pingNb++;
		send_packet(&data, sockFd);
		rpack = recieve_packet(&data, sockFd);
		print_output(&data, rpack);
		usleep(ONE_SEC);
	}
	// // display stats?
	return 0;
}
