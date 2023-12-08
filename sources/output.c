#include "../includes/ft_ping.h"
#include <netinet/in.h>
#include <netinet/ip.h>

void print_output(t_pingData *data) {
	char *revDns = "REVDNS TODO";
	int ttl = data->spacket->ipHeader.ttl;
	char *time = "time";

	if (data->pingNb == 1) {
		printf("PING %s (%s) %d(%d) bytes of data\n", stats.nameDestination, data->strIp,
		 ICMP_PAYLOAD_SIZE, PACKET_SIZE);
	}
	printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n", data->reverseDns, data->strIp, data->pingNb, ttl, data->time);
}
