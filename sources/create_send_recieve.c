#include "../includes/ft_ping.h"
#include <asm-generic/socket.h>
#include <bits/types/struct_iovec.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

unsigned short checksum(void *b, int len)
{
	unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void construct_headers(t_packetData *packet, t_pingData *data) {
	struct iphdr *ipHdr = &(packet->ipHeader);
	struct icmphdr *icmpHdr = &(packet->icmpHeader);

	ipHdr->ihl = 5;
	ipHdr->version = 4;
	ipHdr->tos = 0;
	ipHdr->tot_len = htons(86);
	ipHdr->id = 0;
	ipHdr->frag_off = htons(0);
	ipHdr->ttl = (data->ttl != 0) ? data->ttl : IPDEFTTL;
	ipHdr->protocol = IPPROTO_ICMP;
	ipHdr->check = 0;
	ipHdr->saddr = INADDR_ANY;
	ipHdr->daddr = data->networkIp->sin_addr.s_addr;

	icmpHdr->type = ICMP_ECHO;
	icmpHdr->code = 0;
	icmpHdr->un.echo.id = getpid();
	icmpHdr->un.echo.sequence = data->pingNb;
	icmpHdr->checksum = 0;

	size_t size_pack = sizeof(packet->payload) + sizeof(struct icmphdr);
	icmpHdr->checksum = checksum(&(packet->icmpHeader), size_pack);
}


void construct_packet(t_pingData *data) {
	t_packetData packet;

	strcpy(packet.payload, "patato|potato");
	construct_headers(&packet, data);
	data->packet = packet;
}

int create_socket(t_pingData *data) {
	int sockFd;
	int protocol = IPPROTO_ICMP;

	struct timeval tval;
	ft_memset(&tval, 0, sizeof(tval));
	tval.tv_sec = 10;
	tval.tv_usec = 0;

	sockFd = socket(AF_INET, SOCK_RAW, protocol);
	// !! needs sudo privileges to create raw socket
	if (sockFd < 0) {
		perror("socket");
		exit(1);
	}
	protocol = IPPROTO_IP;
	int optval = 1;
	setsockopt(sockFd, protocol, IP_HDRINCL, (int[1]){1}, sizeof(int));
	setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tval, sizeof(tval));
	
	// char *packetInstance = (char *)malloc(sizeof(t_packetData));
	// ft_memcpy(packetInstance, (void *)data->packet, sizeof(data->packet));
	// printf("size of my packet = %lu\n", sizeof(data->packet));
	
	return sockFd;
}

void send_packet(t_pingData *data, int sockFd) {
	/* printf("size packet envoye %lu\n", sizeof(data->packet)); */

	int bytesSent = sendto(sockFd, &data->packet, sizeof(data->packet),
		    0, (struct sockaddr *)data->networkIp, sizeof(struct sockaddr_in));
	if (bytesSent < 0) {
		printf("this was at this moment jackson knew... he fucked up\n");
		perror("bytes not sent");
	}
}

char* recieve_packet(t_pingData *data, int sockFd) {
	char recieve[PACKET_SIZE];
	struct iovec retMsgData;
	struct msghdr retMsg;

	ft_memset(recieve, 0, PACKET_SIZE);
	ft_memset(&retMsgData, 0, sizeof(retMsgData));
	ft_memset(&retMsg, 0, sizeof(retMsg));

	retMsgData.iov_base = &recieve;
	retMsgData.iov_len = PACKET_SIZE;

	retMsg.msg_iov = &retMsgData;
	retMsg.msg_iovlen = 1;

	ssize_t bytesRecieved = recvmsg(sockFd, &retMsg, 0);
	if (bytesRecieved < 0) {
		printf("this was at this moment jackson junior knew... he fucked up\n");
		perror("bytes not sent");
	}
	// printf("bytes recieved =  %lu \n", bytesRecieved);
	// 
	// print_memory(&recieve, PACKET_SIZE, 16);
	return strdup(recieve);
}

int create_packet(t_pingData *data) {
	int sockFd;

	construct_packet(data);
	sockFd = create_socket(data);

	return sockFd;
}
