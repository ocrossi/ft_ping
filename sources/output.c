#include "../includes/ft_ping.h"
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

 

void print_output(t_pingData *data) {
	int ttl = data->spacket->ipHeader.ttl;
	int precision = get_milisec_precision(data);

	if (stats.pingNb == 1) {
		printf("PING %s (%s) %d(%d) bytes of data\n", stats.nameDestination, data->strIp,
		 ICMP_PAYLOAD_SIZE, PACKET_SIZE);
	}
	dprintf(1, "64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.*lf ms\n", data->reverseDns, data->strIp, stats.pingNb, ttl, precision, data->time);
}
