#include "../includes/ft_ping.h"

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
}

int init(t_pingData *data) {
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

void signal_handler(int signal) { stats.alive = false; }

void set_signals() {
  signal(SIGINT, &signal_handler);
  signal(SIGALRM, &signal_handler);
}

int main(int ac, char **av) {
  t_pingData data;
  int sockFd;
  bool recieved;

  ft_memset(&stats, 0, sizeof(stats));
  ft_memset(&data, 0, sizeof(t_pingData));
  set_signals();
  stats.alive = true;
  parsing(ac, av, &data);
  sockFd = init(&data);
  if (data.interval <= 20000) {
    free(data.networkIp);
    free(stats.median_arr);
    print_flood_protection(&data);
  }
  while (stats.alive) {
    recieved = false;
    stats.nbErrs++;
    send_packet(&data, sockFd);
    recieved = recieve_packet(&data, sockFd);
    if (recieved) {
      manage_time(&data);
      stats.nbErrs--;
      if (check_packet_data(&data)) {
        stats.recieved++;
        data.retPrintSize = ICMP_PAYLOAD_SIZE + sizeof(struct icmphdr); // ???
        if (data.isDomain == true)
          reverseDNS(&data);
        else
          data.reverseDns = ft_strdup(stats.nameDestination);
        if (!(data.options & 32)) // quiet mode
          print_output_loop(&data);
      } else {
        print_output_loop_error(&data);
      }
      if (data.max_ping != 0 && stats.pingNb + 1 == data.max_ping)
        stats.alive = false;
    }
    free_loop_data(&data);
    usleep(data.interval);
    stats.pingNb++;
  }
  print_stats();
  close(sockFd);
  return 0;
}
