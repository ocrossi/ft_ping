#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>

char acceptedFlags[] = "v?ctW";
t_statData stats;


// so apparently im forced to go global here
// https://stackoverflow.com/questions/6970224/providing-passing-argument-to-signal-handler

void print_stats(int signum) {
	printf("\nSTAAAAAAATS\n");
	exit(1);
}

void reverseDNS(t_pingData *data) {
	struct sockaddr_in temp;
	socklen_t len;
	char buff[NI_MAXHOST];

	
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = data->rpacket->ipHeader.saddr;
	len = sizeof(temp);
	if (getnameinfo((struct sockaddr*)&temp, len, buff, sizeof(buff), NULL, 0, NI_NAMEREQD)) {
		printf("DNS error\n");
	} else {
		data->reverseDns = ft_strdup(buff);
	}
}


void manage_time(t_pingData *data) {
	int microsecs = data->recieveTime.tv_usec - data->sendTime.tv_usec;
	char *ret = ft_strdup(ft_itoa(microsecs));
	int len = ft_strlen(ret);
	float milisecs = (float)microsecs / 1000;
	double fMilli = (double)microsecs / 1000;
	data->time = fMilli;
	stats.max = (stats.max > fMilli) ? stats.max : fMilli;
	stats.min = (stats.min > fMilli) ? stats.min : fMilli;
	stats.average = (data->pingNb) == 1 ? stats.average : (stats.average + fMilli) / data->pingNb;
}

int main(int ac, char** av) {
	t_pingData data;
	int sockFd;

	ft_memset(&stats, 0, sizeof(stats));
	ft_memset(&data, 0, sizeof(data));
	parsing(ac, av, &data);
	signal(SIGINT, &print_stats);
	sockFd = create_socket(&data);
	construct_packet(&data);
	while (1) {
		if (data.max_ping != 0 && data.pingNb == data.max_ping) 
			break;
		data.pingNb++;
		send_packet(&data, sockFd);
		recieve_packet(&data, sockFd);
		manage_time(&data);
		reverseDNS(&data);
		print_output(&data);
		usleep(ONE_SEC);
	}
	// // display stats?
	return 0;
}
