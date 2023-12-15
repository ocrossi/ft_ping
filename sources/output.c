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
	printf("-q %30s quiet output\n", "");
	printf("-v %30s verbose output\n", "");
	printf("-c <count> %22s stop sending packet after <count> replies\n", "");
	printf("-t <count> %22s set ttl value to <count>\n", "");
	printf("-i <count> %22s wait <count> seconds between sending each packet\n", "");
	printf("-? %30s Print usage\n", "");
	exit(1);
}


char*	outControlIp(char *in, bool control) {
	size_t len = ft_strlen(in) + 2;

	if (control == true) {
		char *res = ft_strnew(len);
		res[0] = '(';
		ft_strcpy(&res[1], in);
		res[len - 1] = ')';
		res[len] = '\0';
		return res;
	}
	return ft_strdup("");
}

void print_flood_protection() {
	char *s = "TODO";

	dprintf(1, "ping: option value too small: %s\n", s);
	exit(1);
}

void print_output_loop_error(t_pingData *data) {
	if (data->options & 1) {
		printf("%lu size pack; do error verbose here hexdump packet\n", sizeof(*data->rpacket));

		// problem packet size

		return;
	}
	char *ip = outControlIp(data->strIp, data->isDomain);
	char *ipReverseDNS = get_ip_reverseDNS(data);
	char *reverseDNS = (data->reverseDns != NULL) ? data->reverseDns : ipReverseDNS;
	unsigned long len_pack = sizeof(*(data->rpacket));
	dprintf(1, "%lu from %s %s: icmp_seq=%d %s", len_pack, reverseDNS, ip, stats.pingNb, data->error);
	free(ipReverseDNS);

}

void print_output_loop(t_pingData *data) {
	int ttl = data->rpacket->ipHeader.ttl;
	int precision = get_milisec_precision(data);
	
	char *ip = outControlIp(data->strIp, data->isDomain);


	int id = data->rpacket->icmpHeader.un.echo.id;
	if (data->options & 1) {
		dprintf(1, "64 bytes from %s %s: icmp_seq=%d ident=%d ttl=%d time=%.*lf ms\n", data->reverseDns, ip, stats.pingNb, id,ttl, precision, data->time);
	} else {
		dprintf(1, "64 bytes from %s %s: icmp_seq=%d ttl=%d time=%.*lf ms\n", data->reverseDns, ip, stats.pingNb, ttl, precision, data->time);
	}
	// } else {
	// 	char *ipReverseDNS = get_ip_reverseDNS(data);
 // 		char *reverseDNS = (data->reverseDns != NULL) ? data->reverseDns : ipReverseDNS;
	// 	dprintf(1, "From %s (%s): icmp_seq=%d %s", reverseDNS, ipReverseDNS, stats.pingNb, data->error);
	// 	free(ipReverseDNS);
	// }
	free(ip);
}

void print_stats(int signum) {
	double loss = ((stats.transmitted - stats.recieved) / stats.transmitted) * 100;
	int loss_precision = (stats.transmitted == stats.recieved || stats.transmitted == stats.nbErrs) ? 0 : 3;
	stats.average = find_average();
	double stddev = find_stddev();
	double time = convert_to_milliseconds(stats.data->recieveTime, stats.data->start_time); 

	printf("\n--- %s ping statistics ---\n", stats.nameDestination);
	if (stats.nbErrs == 0) {
		printf("%d packets transmitted, %d received, %.*lf%% packet loss\n", stats.transmitted, stats.recieved, loss_precision,loss);
	} else {
		printf("%d packets transmitted, %d received, %.*lf%% packet loss\n", stats.transmitted, stats.recieved, loss_precision,loss);
	}
	if (stats.nbErrs != stats.pingNb)
		printf("round-trip min/avg/max/stddev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", stats.min, stats.average, stats.max, stddev);
	free(stats.median_arr);
	free(stats.data->spacket);
	free(stats.data->strIp);
	free(stats.data->networkIp);

	exit(1);
}

void print_head(t_pingData *data) {
	char* ip = outControlIp(data->strIp, true);

	dprintf(1, "PING %s %s: %d data bytes", stats.nameDestination, ip, ICMP_PAYLOAD_SIZE);
	if (data->options & 1) {
		int id = getpid();
		dprintf(1, ", id = 0x%x = %d", id, id);
	}
	printf("\n");
	free(ip);
}
