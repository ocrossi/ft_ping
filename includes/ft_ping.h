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
#define MAX_ARR_MEDIAN 10000
#define ICMP_PAYLOAD_SIZE 56
#define MAX_PACKET_SIZE 255
#define ONE_SEC 1000000

// errors //
#define EICMP_DEST_UNREACH		"Destination Unreachable\n"
#define EICMP_SOURCE_QUENCH		"Source Quench\n"
#define EICMP_REDIRECT			"Redirect (change route)\n"
#define EICMP_TIME_EXCEEDED		"Time to live exceeded\n"
#define EICMP_PARAMETERPROB		"Parameter Problem\n"
#define EICMP_TIMESTAMP			"Timestamp Request"
#define EICMP_TIMESTAMPREPLY	"Timestamp Reply\n"
#define EICMP_INFO_REQUEST		"Information Request\n"
#define EICMP_INFO_REPLY		"Information Reply\n"
#define EICMP_ADDRESS			"Address Mask Request\n"
#define EICMP_ADDRESSREPLY		"Address Mask Reply\n"

#define	UNKNOWN_ERR_CODE		"Unknown error code\n" 

typedef struct timeval		t_val;

typedef struct		s_packetData {
	struct iphdr	ipHeader;						//size 20
	struct icmphdr	icmpHeader;						// size 8
	char			payload[ICMP_PAYLOAD_SIZE];
}					t_packetData;						// total is 84

typedef struct 	s_pingData {
	char					options; // bits are 1 if acceptedFlag[bit_pos] est dans input
	char					*strIp;
	struct sockaddr_in		*networkIp;
	char					*reverseDns;
	t_packetData			*spacket;
	t_packetData			*rpacket;
	int						max_ping;
	int						ttl;
	useconds_t				interval;
	t_val					sendTime;
	t_val					recieveTime;
	t_val					start_time;
	double					time;
	char					*error;
	bool					isDomain;
}							t_pingData;


typedef struct 	s_statData {
	t_pingData				*data;
	char					*nameDestination;
	int						transmitted;
	int						recieved;
	int						lost;
	int						pingNb;
	double					time;
	double					min;
	double					average;
	double					median;
	double					max;
	double					mdev;
	int						nbErrs;
	double					*median_arr;
}							t_statData;



extern char						acceptedFlags[];
extern t_statData				stats;

void			parsing(int ac, char **args, t_pingData *data);
int				create_socket(t_pingData *data);
void			construct_packet(t_pingData *data);
void			send_packet(t_pingData *data, int sockFd);
bool			recieve_packet(t_pingData *data, int sockFd);
void			print_output_loop(t_pingData *data);
void			print_output_loop_error(t_pingData *data);
void			print_usage(char invalidFlag);
void			print_stats(int signum);
void			print_head(t_pingData *data);
void			print_flood_protection(void);
void			reverseDNS(t_pingData *data);
void			set_median_arr(void);
double			find_average(void);
double			find_stddev(void);
double			convert_to_milliseconds(t_val time, t_val base);
bool			check_packet_data(t_pingData *data);
void			manage_time(t_pingData *data);
char*			get_ip_reverseDNS(t_pingData *data);
useconds_t		convert_to_microseconds(double seconds);

#endif
