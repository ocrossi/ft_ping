#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
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
	reverseDNS(data);
	char *ip = outControlIp(data->strIp, data->isDomain);
	char *reverseDNS = (data->reverseDns != NULL) ? data->reverseDns : get_ip_reverseDNS(data);
	if (data->reverseDns != NULL && data->rpacket->icmpHeader.type == 11) {
		dprintf(1, "do we get error bytes here?\n");
	}
	// dprintf(1, "wait what? in print output loop error fimme strings\n");
	// dprintf(1, "ip = %s\n", ip);
	// dprintf(1, "ipReverseDNS %s\n", ipReverseDNS);
	// dprintf(1, "reverseDNS %s\n", reverseDNS);
	// dprintf(1, "wait what? in print output loop error fimme strings\n");
	dprintf(1, "%d bytes from %s %s: icmp_seq=%d %s", data->retPrintSize, reverseDNS, ip, stats.pingNb, data->error);
	if (data->options & 1) {
		struct iphdr *ret = (t_ipHdr *)&data->recievedBytesArray[28];
		char flag = data->recievedBytesArray[28 + 6] >> 5;
		char srcAddr[INET_ADDRSTRLEN];	
		char dstAddr[INET_ADDRSTRLEN];
		struct in_addr ip;
		printf("IP Hdr Dump:\n");
		print_memory(&data->recievedBytesArray[28], 20, 16);
		printf("\nVr HL TOS  Len   ID Flg   off TTL Pro  cks	Src		Dst		Data\n");
		dprintf(1, " %hhd  %hhd  ", ret->version, ret->ihl);
		print_memory(&data->recievedBytesArray[28 + 1] , 1, 16);
		dprintf(1, " ");
		print_memory(&data->recievedBytesArray[28 + 2] , 2, 16);
		print_memory(&data->recievedBytesArray[28 + 4] , 2, 16);
		dprintf(1, "  %hhd ", flag);
		dprintf(1, " %04hx ", data->recievedBytesArray[28 + 7]);
		dprintf(1, " %02hhd ", data->recievedBytesArray[28 + 8]);
		dprintf(1, " %02hhd ", data->recievedBytesArray[28 + 9]);
		dprintf(1, " %04hx", ntohs(*((short *)&data->recievedBytesArray[28 + 10])));
		ip.s_addr = *((int *)&data->recievedBytesArray[28+12]);
		inet_ntop(AF_INET, &ip, srcAddr, INET_ADDRSTRLEN);
		dprintf(1, "\t%s\t", srcAddr);
		ip.s_addr = *((int *)&data->recievedBytesArray[28+16]);
		inet_ntop(AF_INET, &ip, dstAddr, INET_ADDRSTRLEN);
		dprintf(1, "%s\n", dstAddr);
		dprintf(1, "ICMP: type %hhd, code %hhu, size %hu, id 0x%hx, seq 0x%04hx", data->recievedBytesArray[28+20],
		  data->recievedBytesArray[28+21], sizeof(struct icmphdr) + ICMP_PAYLOAD_SIZE , getpid(), stats.pingNb - 1);
	}
	free(ip);
}

void print_output_loop(t_pingData *data) {
	int ttl = data->rpacket->ipHeader.ttl;
	int precision = get_milisec_precision(data);
	
	char *ip = outControlIp(data->strIp, data->isDomain);


	// if (data->options & 1) {
	dprintf(1, "64 bytes from %s %s: icmp_seq=%d ttl=%d time=%.*lf ms\n", data->reverseDns, ip, stats.pingNb, ttl, precision, data->time);
	// } else {
	// 	dprintf(1, "64 bytes from %s %s: icmp_seq=%d ttl=%d time=%.*lf ms\n", data->reverseDns, ip, stats.pingNb, ttl, precision, data->time);
	// }

	free(ip);
}

void print_stats(int signum) {
	double loss = ((stats.transmitted - stats.recieved) / stats.transmitted) * 100;
	int loss_precision = (stats.transmitted == stats.recieved || stats.transmitted == stats.nbErrs) ? 0 : 3;
	stats.average = find_average();
	double stddev = find_stddev();
	double time = convert_to_milliseconds(stats.data->recieveTime, stats.data->start_time); 

	printf("--- %s ping statistics ---\n", stats.nameDestination);
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

void print_alarm(int signal) {
  (void)signal;

  fprintf(stderr, "alarm signal spotted\n");
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
