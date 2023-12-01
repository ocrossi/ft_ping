#include "../includes/ft_ping.h"
#include <bits/types/struct_iovec.h>
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

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
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
	ipHdr->frag_off = 0;
	ipHdr->ttl = IPDEFTTL; // can inc to MAXTTL
	ipHdr->protocol = IPPROTO_ICMP;
	ipHdr->check = 0;
	ipHdr->saddr = INADDR_ANY;
	ipHdr->daddr = data->networkIp->sin_addr.s_addr;

	icmpHdr->type = ICMP_ECHO;
	icmpHdr->code = 0;
	icmpHdr->un.echo.id = getpid();
	icmpHdr->un.echo.sequence = data->pingNb;

	icmpHdr->checksum = checksum(packet->payload, sizeof(packet->payload));
}


void construct_packet(t_pingData *data) {
	t_packetData packet;

	strcpy(packet.payload, "patato|potato");
	construct_headers(&packet, data);
	data->packet = packet;
}

int create_socket(t_pingData *data) {
	int sockFd;
	int protocol = IPPROTO_RAW;

	sockFd = socket(AF_INET, SOCK_RAW, protocol);
	// !! needs sudo privileges to create raw socket
	if (sockFd < 0) {
		perror("socket");
		exit(1);
	}
	setsockopt(sockFd, IPPROTO_IP, IP_HDRINCL, &protocol, sizeof(protocol));
	
	// char *packetInstance = (char *)malloc(sizeof(t_packetData));
	// ft_memcpy(packetInstance, (void *)data->packet, sizeof(data->packet));
	
	printf("size of my packet = %lu\n", sizeof(data->packet));

	return sockFd;
}

void send_packet(t_pingData *data, int sockFd) {
	int bytesSent = sendto(sockFd, &data->packet, sizeof(data->packet),
		    0, (struct sockaddr *)data->networkIp, sizeof(struct sockaddr_in));
	if (bytesSent < 0) {
		printf("this was at this moment jackson knew... he fucked up\n");
		perror("bytes not sent");
	}
}

void recieve_message(t_pingData *data, int sockFd) {
	struct iovec pigeon_sickness = {
		.iov_base = &(data->packet),
		.iov_len = sizeof(data->packet),
	};
	struct msghdr pigeon = {
		// .msg_name = (void *)data->networkIp,
		// .msg_namelen = sizeof(struct sockaddr_in),
		.msg_iov = &pigeon_sickness,//il est malade le pigeon wesh
		.msg_iovlen = 1,
		// .msg_controllen = NULL,
		// .msg_flags = 0,
	};
	printf("test \n");
	int bytesRecieved = recvmsg(sockFd, &pigeon, 0);
	printf("test \n");
	if (bytesRecieved < 0) {
		printf("this was at this moment jackson junior knew... he fucked up\n");
		perror("bytes not sent");
	}
}

void create_and_send_packet(t_pingData *data) {
	data->pingNb++; // might be the only thing to change when sending multiple packets

	construct_packet(data);
	int sockFd = create_socket(data);
	send_packet(data, sockFd);
	recieve_message(data, sockFd);

}
