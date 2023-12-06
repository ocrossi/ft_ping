#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>

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
	char buff[100];

	
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = inet_addr(data->strIp);
	len = sizeof(temp);

	if (getnameinfo((struct sockaddr*)&temp, len, buff, 100, NULL, 0, NI_NAMEREQD)) {
		printf("DNS error\n");
	} else {
		data->reverseDns = ft_strdup(buff);
	}
}


int main(int ac, char** av) {
	t_pingData data;
	int sockFd;
	char *rpack;

	ft_memset(&stats, 0, sizeof(stats));
	ft_memset(&data, 0, sizeof(data));
	parsing(ac, av, &data);
	signal(SIGINT, &print_stats);
	sockFd = create_packet(&data);
	reverseDNS(&data);
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
