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
	printf("-c <count> % 22s stop sending packet after <count> replies\n", "");
	printf("-t <count> % 22s set ttl value to <count>\n", "");
	printf("-W <count> % 22s time to wait for response\n", "");
	printf("-? % 30s Print usage\n", "");
	exit(1);
}

void print_flood_protection() {
	dprintf(1, "ping: cannot flood; minimal interval allowed for user is 2ms\n");
	exit(1);
}

void print_output_loop(t_pingData *data, bool recieved) {
	int ttl = data->rpacket->ipHeader.ttl;
	int precision = get_milisec_precision(data);
	
	char *ip = data->strIp;


	int id = data->rpacket->icmpHeader.un.echo.id;
	if (recieved == true) {
		if (data->options & 1) {
			dprintf(1, "64 bytes from %s (%s): icmp_seq=%d ident=%d ttl=%d time=%.*lf ms\n", data->reverseDns, data->strIp, stats.pingNb, id,ttl, precision, data->time);
		} else {
			dprintf(1, "64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.*lf ms\n", data->reverseDns, data->strIp, stats.pingNb, ttl, precision, data->time);
		}
	} else {
		char *ipReverseDNS = get_ip_reverseDNS(data);
 		char *reverseDNS = (data->reverseDns != NULL) ? data->reverseDns : ipReverseDNS;
		dprintf(1, "From %s (%s): icmp_seq=%d %s", reverseDNS, ipReverseDNS, stats.pingNb, data->error);
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
	free(stats.data->networkIp);
	
	exit(1);
}

void print_head(t_pingData *data) {
	if (data->options & 1) {
		dprintf(1, "ping: sock4.fd: 3 (socktype: SOCK_RAW), sock6.fd: 4 (socktype: SOCK_RAW), hints.ai_family: AF_UNSPEC \n\n");
		dprintf(1, "ai->ai_family: AF_INET, ai->ai_canonname: '%s'\n", stats.nameDestination);
	}
	dprintf(1, "PING %s (%s) %d(%d) bytes of data\n\n", stats.nameDestination, data->strIp, ICMP_PAYLOAD_SIZE, PACKET_SIZE);
}
