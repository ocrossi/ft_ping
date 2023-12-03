#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
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
#define MAX_INT 2147483647
#define PACKET_SIZE 84
#define ICMP_PAYLOAD_SIZE 56
#define MAX_PACKET_SIZE 255
#define ONE_SEC 1000000

typedef struct		s_packetData {
	struct iphdr	ipHeader;						//size 20
	struct icmphdr	icmpHeader;						// size 8
	char			payload[ICMP_PAYLOAD_SIZE];
}					t_packetData;						// total is 84

typedef struct 	s_pingData {
	char					options; // bits are 1 if acceptedFlag[bit_pos] est dans input
	char					*strIp;
	struct sockaddr_in		*networkIp;
	int						pingNb;
	t_packetData			packet;
	int						max_ping;
	int						ttl;
	int						timeout;
}							t_pingData;


typedef struct 	s_statData {
	char					*nameDestination;
	int						transmitted;
	int						recieved;
	int						lost;
	time_t					time;
	time_t					min;
	time_t					average;
	time_t					max;
	time_t					mdev;
}							t_statData;



extern char		acceptedFlags[];
t_pingData		parsing(int ac, t_statData *stats, char **args);
int				create_packet(t_pingData *data);
void			send_packet(t_pingData *data, int sockFd);
char*			recieve_packet(t_pingData *data, int sockFd);
void			print_output(t_pingData *data, char *packet);


#endif
