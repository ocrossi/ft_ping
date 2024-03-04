#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>

char acceptedFlags[] = "v?ctiq";
t_statData stats;

void free_loop_data(t_pingData *data) {
  if (data->reverseDns != NULL) {
    free(data->reverseDns);
    data->reverseDns = NULL;
  }
  if (data->rpacket != NULL) {
    free(data->rpacket);
    data->rpacket = NULL;
  }
  if (data->recievedBytesArray != NULL) {
    free(data->recievedBytesArray);
    data->recievedBytesArray = NULL;
  }
  if (data->max_ping != 0 && stats.pingNb == data->max_ping) {
    print_stats();
    exit(0);
  }
}

int init(t_pingData *data) {

  dprintf(1, "init\n");

  int sockFd = create_socket();
  construct_packet(data);
  stats.median_arr = (double *)malloc(sizeof(double) * MAX_ARR_MEDIAN);
  stats.data = data;
  set_median_arr();
  gettimeofday(&data->start_time, NULL);
  print_head(data);
  data->interval = data->interval == 0 ? ONE_SEC : data->interval;
  return sockFd;
}

void sigint_handler(int signal) {
  if (signal == SIGQUIT) {
    printf("got sigquit\n");
    exit(0);
  }
  if (signal == SIGINT) {
    printf("got sigint\n");
    stats.alive = false;
    print_stats();
  }
}

void set_signal_action() {
  struct sigaction act;

  ft_bzero(&act, sizeof(struct sigaction));

  act.sa_handler = sigint_handler;

  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  if (sigaction(SIGINT, &act, NULL) == -1) {
    perror("sigaction error");
    exit(1);
  }
  if (sigaction(SIGQUIT, &act, NULL) == -1) {
    perror("sigquit error");
    exit(1);
  }
}

void set_signals() {
  signal(SIGINT, &print_stats);
  signal(SIGALRM, &print_alarm);
}

int main(int ac, char **av) {
  t_pingData data;
  int sockFd;
  bool recieved;
  bool error;

  ft_memset(&stats, 0, sizeof(stats));
  ft_memset(&data, 0, sizeof(t_pingData));
  set_signal_action();
  stats.alive = true;
  if (stats.alive)
    parsing(ac, av, &data);
  if (stats.alive)
    sockFd = init(&data);
  if (data.interval <= 20000) {
    free(data.networkIp);
    free(stats.median_arr);
    print_flood_protection(&data);
  }
  while (1 || stats.alive) {
    recieved = false;
    error = false;
    stats.pingNb++;
    stats.nbErrs++;
    send_packet(&data, sockFd);
    if (recieve_packet(&data, sockFd)) {
      manage_time(&data);
      recieved = true;
      stats.nbErrs--;
    } else {
      dprintf(1, "didnt recieve packet\n");
      continue;
    }
    if (recieved && !check_packet_data(&data)) {
      error = true;
      print_output_loop_error(&data);
    } else {
      stats.recieved++;
      data.retPrintSize = ICMP_PAYLOAD_SIZE + sizeof(struct icmphdr);
    }
    if (data.isDomain == true)
      reverseDNS(&data);
    else
      data.reverseDns = ft_strdup(stats.nameDestination);
    if (!(data.options & 32) && !error &&
        recieved) { // if quiet is not activated
      print_output_loop(&data);
    }
    free_loop_data(&data);
    usleep(data.interval);
  }
  close(sockFd);
  return 0;
}
