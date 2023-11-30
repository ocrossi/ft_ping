#ifndef FT_PING_H
# define FT_PING_H

// #define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>


#include <netdb.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "../libraries/libft.h"
#define CPT_FLAGS 2
// #define SOCK_PORT 4242

#define ICMP_PAYLOAD_SIZE 56
#define MAX_PACKET_SIZE 255



typedef struct		s_packetData {
	struct iphdr	ipHeader;						//size 20
	struct icmphdr	icmpHeader;						// size 8
	char			payload[ICMP_PAYLOAD_SIZE];
}					t_packetData;						// total is 84

typedef struct 	s_pingData {
	bool					options[CPT_FLAGS];
	char					*strIp;
	struct sockaddr_in		*networkIp;
	int						pingNb;
	t_packetData			packet;
}							t_pingData;


t_pingData		parsing(int ac, char **args);
void			create_and_send_packet(t_pingData *data);


#endif
