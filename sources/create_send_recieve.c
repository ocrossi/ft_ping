#include "../includes/ft_ping.h"
#include <netinet/ip.h>
#include <unistd.h>

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
	ipHdr->tot_len = htons(PACKET_SIZE);
	ipHdr->id = getpid();
	ipHdr->frag_off = htons(0x4000);
	ipHdr->ttl = (data->ttl != 0) ? data->ttl : IPDEFTTL;
	ipHdr->protocol = IPPROTO_ICMP;
	ipHdr->check = 0;
	ipHdr->saddr = INADDR_ANY;
	ipHdr->daddr = data->networkIp->sin_addr.s_addr;

	icmpHdr->type = ICMP_ECHO;
	icmpHdr->code = 0;
	icmpHdr->un.echo.id = getpid();
	icmpHdr->un.echo.sequence = stats.pingNb;
	icmpHdr->checksum = 0;

	size_t size_pack = sizeof(packet->payload) + sizeof(struct icmphdr);
	icmpHdr->checksum = checksum(&(packet->icmpHeader), size_pack);
}


void construct_packet(t_pingData *data) {
	t_packetData *packet = (t_packetData *)malloc(sizeof(t_packetData));

	strcpy(packet->payload, "patato|potato");
	construct_headers(packet, data);
	data->spacket = packet;
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

	return sockFd;
}

void send_packet(t_pingData *data, int sockFd) {
	gettimeofday(&data->sendTime, NULL);

	int bytesSent = sendto(sockFd, data->spacket, sizeof(t_packetData),
		    0, (struct sockaddr *)data->networkIp, sizeof(struct sockaddr_in));
	if (bytesSent < 0) {
		perror("bytes not sent");
	} else {
		stats.transmitted++;
	}
}

bool recieve_packet(t_pingData *data, int sockFd) {
	char recieve[MAX_PACKET_SIZE];
	struct iovec retMsgData;
	struct msghdr retMsg;

	ft_memset(recieve, 0, MAX_PACKET_SIZE);
	ft_memset(&retMsgData, 0, sizeof(retMsgData));
	ft_memset(&retMsg, 0, sizeof(retMsg));

	retMsgData.iov_base = &recieve;
	retMsgData.iov_len = MAX_PACKET_SIZE;

	retMsg.msg_iov = &retMsgData;
	retMsg.msg_iovlen = 1;

	ssize_t bytesRecieved = recvmsg(sockFd, &retMsg, 0);
	if (bytesRecieved < 0) {
		// printf("no bytes recieved\n");
		return false;
	}
	else {
		// printf("bytes recieved %lu\n", bytesRecieved);
		// print_memory(recieve, bytesRecieved, 16);

		// for (int i = 0; i < bytesRecieved; i++) {
		// 	if (recieve[i] == 0) {
		// 		printf("we have a 00 byte at index %d\n", i);
		// 	}
		// }
	}
	data->rpacket = (t_packetData *)malloc(bytesRecieved);
	ft_memcpy(data->rpacket, recieve, bytesRecieved);
	data->recievedBytesArray = (char *)malloc(bytesRecieved);
	ft_memcpy(data->recievedBytesArray, recieve, bytesRecieved);
	gettimeofday(&data->recieveTime, NULL);
  data->retPrintSize = bytesRecieved;
  return true;
}
