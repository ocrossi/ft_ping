#include "../includes/ft_ping.h"
#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

// void create_socket_connection(t_pingData *data) {
// 	int socketFd, socketFdTest;
// 	struct sockaddr_in servAddr;
// 	char buffer[1024];
// 	int recvlen;
//
// 	socketFd = socket(AF_INET, SOCK_STREAM, 0);
// 	socketFdTest = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
// 	// socket pipeline with tcp headers i guess
// 	// we need our own raw socket composed of 
// 	// our custom ip header + icmp packet
//
// 	printf("my socketFd is %d\n", socketFd);
// 	if (socketFd < 0) {
// 		printf("socket connection with stream refused\n");
// 		exit(1);
// 	}
// 	if (socketFdTest < 0) {
// 		printf("socket connection with raw proto refused\n");
// 		exit(1);
// 	}
// 	servAddr.sin_family = AF_INET;
// 	servAddr.sin_port = htons(SOCK_PORT);
// 	servAddr.sin_addr.s_addr = INADDR_ANY;
// }

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

// char* construct_ip_header(t_pingData *data) {
// 	char *ip_header;
// 	char *needle;
// 	char ihl = 4;
// 	// IHL:  4 bits
// 	// Internet Header Length is the length of the internet header in 32
// 	// bit words, and thus points to the beginning of the data.  Note that
// 	// the minimum value for a correct header is 5.
// 	ip_header = ft_strnew(100);
// 	ft_memset(ip_header, 0, 100);
//
// 	ip_header[0] = 1 << 6;
// 	// printf("just version");
// 	// print_memory(ip_header, 1, 2);
// 	ip_header[0] += ihl;
// 	// printf("version + ihl a 4");
// 	// print_memory(ip_header, 1, 2);
// 	ip_header[1] = 1 << 6;
// 	// precedence: 010 - Immediate 
// 	// Bits 0-2:  Precedence.
// 	ip_header[1] += 1 << 4;
//     //    Bit    3:  0 = Normal Delay,      1 = Low Delay.
// 	ip_header[1] += 1 << 3;
//     //    Bits   4:  0 = Normal Throughput, 1 = High Throughput.
// 	ip_header[1] += 1 << 2;
//     //    Bits   5:  0 = Normal Relibility, 1 = High Relibility.
//     //    Bit  6-7:  Reserved for Future Use.
//
// 	ip_header[3] = 10;
// 	// field total length, unknown 4 now, takes bytes pos 2&3
// 	
// 	ip_header[4] = 0x06;
// 	ip_header[5] = 0x66;
// 	// identification, unknow yet
// 	
// 	ip_header[6] = 1 << 6;
// 	// flags 
// 	// Bit 0: reserved, must be zero
// 	// Bit 1: (DF) 0 = May Fragment,  1 = Don't Fragment.
// 	// Bit 2: (MF) 0 = Last Fragment, 1 = More Fragments.
// 	// we skip fragment offset coz pings in 1 packet
// 	ip_header[8] = 255;
// 	// max ttl for now, well see later
// 	ip_header[9] = 1;
// 	// protocol icmp
// 	ip_header[10] = 0xff;
// 	ip_header[11] = 0xff;
// 	// here use checksum function instead
//
// 	print_memory(ip_header, 12, 2);
// 	return ip_header;
// }



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
		&data->packet,
		sizeof(data->packet)
	};
	struct msghdr pigeon = {
		.msg_name = (void *)data->networkIp,
		.msg_namelen = sizeof(struct sockaddr_in),
		.msg_iov = &pigeon_sickness,//il est malade le pigeon wesh
		.msg_iovlen = 1,
		.msg_controllen = NULL,
		.msg_flags = 0,
	};


	int bytesRecieved = recvmsg(sockFd, &pigeon, MSG_CMSG_CLOEXEC);
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
