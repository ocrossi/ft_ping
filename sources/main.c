#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>

char acceptedFlags[] = "v?ctWq";
t_statData stats;

int main(int ac, char** av) {
	t_pingData data;
	int sockFd;
	bool recieved;

	ft_memset(&stats, 0, sizeof(stats));
	ft_memset(&data, 0, sizeof(data));
	parsing(ac, av, &data);
	signal(SIGINT, &print_stats);
	sockFd = create_socket(&data);
	construct_packet(&data);
	stats.median_arr = (double *)malloc(sizeof(double) * MAX_ARR_MEDIAN);
	stats.data = &data;
	set_median_arr();
	gettimeofday(&data.start_time, NULL);
	dprintf(1, "PING %s (%s) %d(%d) bytes of data\n\n", stats.nameDestination, data.strIp,
			ICMP_PAYLOAD_SIZE, PACKET_SIZE);
	while (1) {
		recieved = false;
		if (data.max_ping != 0 && stats.pingNb == data.max_ping) {
			print_stats(0);
			exit(0);
		}
		stats.pingNb++;
		send_packet(&data, sockFd);
		if (recieve_packet(&data, sockFd) && check_packet_data(&data)) {
			stats.recieved++;
			manage_time(&data);
			recieved = true;
		}
		reverseDNS(&data);
		if (!(data.options & 32)) { // if quiet is not activated
			print_output_loop(&data, recieved);
		}
		if (data.reverseDns != NULL)
			free(data.reverseDns);
		if (data.rpacket != NULL) {
			free(data.rpacket);
		}
		usleep(ONE_SEC);
	}
	return 0;
}
