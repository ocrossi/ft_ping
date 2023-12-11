#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>

int get_milisec_precision(t_pingData *data) {
	int digit = 0;
	double time = data->time;


	for(int i = 0; i < 3; i++) {
		if (time / 10 > 0.1) {
			digit++;
			time /= 10;
		}
	}
	return 3 - digit;
}

void print_usage(char invalidFlag) {
	if (invalidFlag != 0)
		fprintf(stderr, "ping: invalid option -- '%c'\n\n", invalidFlag);
	printf("Usage\n\tping [options] <destination>\n\nOptions:\n");
	printf("\t<destination>\t\t  dns name or ip address\n\n");
	printf("-q % 30s quiet output\n", "");
	printf("-v % 30s verbose output\n", "");
	printf("-c <count> % 22s stop spacket <count> replies\n", "");
	printf("-t <count> % 22s set ttl value to <count>\n", "");
	printf("-? % 30s Print usage\n", "");
	exit(1);
}

char *get_ip_reverseDNS(t_pingData *data) {
	char buff[INET_ADDRSTRLEN];
	int ret;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct sockaddr_in *h;
	ft_memset(&hints, 0, sizeof(hints));
	ft_memset(&buff, 0, sizeof(buff));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = 0;

	if ((ret = getaddrinfo(data->reverseDns, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo crashed with %s\n", gai_strerror(ret));
		exit(1);
	}
	h = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
	return ft_strdup(buff);
} 

void print_output_loop(t_pingData *data, bool recieved) {
	int ttl = data->spacket->ipHeader.ttl;
	int precision = get_milisec_precision(data);


	if (recieved == true) {
		dprintf(1, "64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.*lf ms\n", data->reverseDns, data->strIp, stats.pingNb, ttl, precision, data->time);
	} else {
		char *ipReverseDNS = get_ip_reverseDNS(data);
		dprintf(1, "From %s (%s): icmp_seq=%d %s", data->reverseDns, ipReverseDNS, stats.pingNb, data->error);
		free(ipReverseDNS);
	}
}

void print_stats(int signum) {
	double loss = ((stats.transmitted - stats.recieved) / stats.transmitted) * 100;
	int loss_precision = (stats.transmitted == stats.recieved || stats.transmitted == stats.nbErrs) ? 0 : 3;
	stats.average = find_average();
	double stddev = find_stddev();
	double time = convert_to_milliseconds(stats.data->recieveTime, stats.data->start_time); 

	printf("\n--- %s ping statistics ---\n", stats.nameDestination);
	if (stats.nbErrs == 0) {
		printf("%d packets transmitted, %d received, %.*lf%% packet loss, time %.0lfms\n", stats.transmitted, stats.recieved, loss_precision,loss, time);
	} else {

		printf("%d packets transmitted, %d received, +%d errors, %.*lf%% packet loss, time %.0lfms\n", stats.transmitted, stats.recieved, stats.nbErrs, loss_precision,loss, time);
	}
	if (stats.nbErrs != stats.pingNb)
		printf("rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", stats.min, stats.average, stats.max, stddev);
	free(stats.median_arr);
	free(stats.data->spacket);
	free(stats.data->strIp);
	
	exit(1);
}
