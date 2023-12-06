#include "../includes/ft_ping.h"
#include <netinet/in.h>
#include <netinet/ip.h>

void print_output(t_pingData *data, char *packet) {
	char *revDns = "REVDNS TODO";
	int ttl = MAXTTL-packet[9];
	char *time = "time";

	if (data->pingNb == 1) {
		printf("PING %s (%s) %d(%d) bytes of data\n", stats.nameDestination, data->strIp,
		 ICMP_PAYLOAD_SIZE, PACKET_SIZE);
	}
	printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%s\n", data->reverseDns, data->strIp, data->pingNb, ttl, time);
}
