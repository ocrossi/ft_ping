#ifndef FT_PING_H
#define FT_PING_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <netdb.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "../libraries/libft.h"
#define MAX_INT 2147483647
#define PACKET_SIZE 84
#define RET_PACKET_SIZE 999
#define MAX_ARR_MEDIAN 10000
#define ICMP_PAYLOAD_SIZE 56
#define MAX_PACKET_SIZE 255
#define ONE_SEC 1000000
#define OPT_NB 6
// errors //
#define EICMP_DEST_UNREACH "Destination Unreachable\n"
#define EICMP_SOURCE_QUENCH "Source Quench\n"
#define EICMP_REDIRECT "Redirect (change route)\n"
#define EICMP_TIME_EXCEEDED "Time to live exceeded\n"
#define EICMP_PARAMETERPROB "Parameter Problem\n"
#define EICMP_TIMESTAMP "Timestamp Request"
#define EICMP_TIMESTAMPREPLY "Timestamp Reply\n"
#define EICMP_INFO_REQUEST "Information Request\n"
#define EICMP_INFO_REPLY "Information Reply\n"
#define EICMP_ADDRESS "Address Mask Request\n"
#define EICMP_ADDRESSREPLY "Address Mask Reply\n"

#define UNKNOWN_ERR_CODE "Unknown error code\n"

typedef struct timeval t_val;
typedef struct iphdr t_ipHdr;

typedef struct s_packetData {
  struct iphdr ipHeader;
  struct icmphdr icmpHeader;
  char payload[ICMP_PAYLOAD_SIZE];
} t_packetData;

typedef struct s_pingData {
  char acceptedFlags[OPT_NB + 1];
  char options; // bits are 1 if acceptedFlag[bit_pos] est dans input
  char *strIp;
  struct sockaddr_in *networkIp;
  char *reverseDns;
  t_packetData *spacket;
  t_packetData *rpacket;
  char *recievedBytesArray;
  int max_ping;
  int ttl;
  useconds_t interval;
  char *intervalStr;
  t_val sendTime;
  t_val recieveTime;
  t_val start_time;
  double time;
  char *error;
  bool isDomain;
  int retPrintSize;
} t_pingData;

typedef struct s_statData {
  t_pingData *data;
  char *nameDestination;
  int transmitted;
  int recieved;
  int lost;
  int pingNb;
  double time;
  double min;
  double average;
  double median;
  double max;
  double mdev;
  int nbErrs;
  double *median_arr;
  bool alive;
} t_statData;

extern t_statData stats;

/* INIT */
void parsing(int ac, char **args, t_pingData *data);
int create_socket(void);
void construct_packet(t_pingData *data);

/* OUTPUT */
void send_packet(t_pingData *data, int sockFd);
bool recieve_packet(t_pingData *data, int sockFd);
void print_output_loop(t_pingData *data);
void print_output_loop_error(t_pingData *data);
void print_usage(char invalidFlag);
void print_stats();
void print_alarm(int signum);
void print_head(t_pingData *data);
void print_flood_protection(t_pingData *data);

/* HELPERS */
void reverseDNS(t_pingData *data);
void set_median_arr(void);
double find_average(void);
double find_stddev(void);
double convert_to_milliseconds(t_val time, t_val base);
bool check_packet_data(t_pingData *data);
void manage_time(t_pingData *data);
char *get_ip_reverseDNS(t_pingData *data);
useconds_t convert_to_microseconds(double seconds);
int get_milisec_precision(t_pingData *data);

#endif
