#include "../includes/ft_ping.h"

void print_usage(char invalidFlag) {
  if (invalidFlag != 0)
    fprintf(stderr, "ping: invalid option -- '%c'\n\n", invalidFlag);
  dprintf(1, "Usage\n\tping [options] <destination>\n\nOptions:\n");
  dprintf(1, "\t<destination>\t\t  dns name or ip address\n\n");
  dprintf(1, "-q %30s quiet output\n", "");
  dprintf(1, "-v %30s verbose output\n", "");
  dprintf(1, "-c <count> %22s stop sending packet after <count> replies\n", "");
  dprintf(1, "-t <count> %22s set ttl value to <count>\n", "");
  dprintf(1,
          "-i <count> %22s wait <count> seconds between sending each packet\n",
          "");
  dprintf(1, "'?'%30s Print usage\n", "");
  exit(1);
}

char *outControlIp(char *in, bool control) {
  size_t len = ft_strlen(in) + 2;

  if (control == true) {
    char *res = ft_strnew(len);
    res[0] = '(';
    ft_strcpy(&res[1], in);
    res[len - 1] = ')';
    res[len] = '\0';
    return res;
  }
  return ft_strdup("");
}

void print_flood_protection(t_pingData *data) {
  dprintf(1, "ping: option value too small: %s\n", data->intervalStr);
  exit(1);
}

void print_output_loop_error(t_pingData *data) {
  char *ip = outControlIp(data->strIp, data->isDomain);
  char *ipReverseDNS = get_ip_reverseDNS(data);
  char *reverseDNS =
      (data->reverseDns != NULL) ? data->reverseDns : ipReverseDNS;
  dprintf(1, "%d bytes from %s : icmp_seq=%d %s", data->retPrintSize,
          reverseDNS, stats.pingNb, data->error);
  if (data->options & 1) {
    struct iphdr *ret = (t_ipHdr *)&data->recievedBytesArray[28];
    char flag = data->recievedBytesArray[28 + 6] >> 5;
    char srcAddr[INET_ADDRSTRLEN];
    char dstAddr[INET_ADDRSTRLEN];
    struct in_addr ip;
    dprintf(1, "IP Hdr Dump:\n");
    print_memory(&data->recievedBytesArray[28], 20, 16);
    dprintf(1, "\nVr HL TOS  Len   ID Flg   off TTL Pro  cks	Src	"
               "	Dst		Data\n");
    dprintf(1, " %hhd  %hhd  ", ret->version, ret->ihl);
    print_memory(&data->recievedBytesArray[28 + 1], 1, 16);
    dprintf(1, " ");
    print_memory(&data->recievedBytesArray[28 + 2], 2, 16);
    print_memory(&data->recievedBytesArray[28 + 4], 2, 16);
    dprintf(1, "  %hhd ", flag);
    dprintf(1, " %04hx ", data->recievedBytesArray[28 + 7]);
    dprintf(1, " %02hhd ", data->recievedBytesArray[28 + 8]);
    dprintf(1, " %02hhd ", data->recievedBytesArray[28 + 9]);
    dprintf(1, " %04hx", ntohs(*((short *)&data->recievedBytesArray[28 + 10])));
    ip.s_addr = *((int *)&data->recievedBytesArray[28 + 12]);
    inet_ntop(AF_INET, &ip, srcAddr, INET_ADDRSTRLEN);
    dprintf(1, "\t%s\t", srcAddr);
    ip.s_addr = *((int *)&data->recievedBytesArray[28 + 16]);
    inet_ntop(AF_INET, &ip, dstAddr, INET_ADDRSTRLEN);
    dprintf(1, "%s\n", dstAddr);
    dprintf(
        1, "ICMP: type %hhd, code %hhu, size %lu, id 0x%hx, seq 0x%04hx\n",
        data->recievedBytesArray[28 + 20], data->recievedBytesArray[28 + 21],
        sizeof(struct icmphdr) + ICMP_PAYLOAD_SIZE, getpid(), stats.pingNb - 1);
  }
  free(ipReverseDNS);
  free(ip);
}

void print_output_loop(t_pingData *data) {
  int ttl = data->rpacket->ipHeader.ttl;
  int precision = get_milisec_precision(data);
  char *ip = outControlIp(data->strIp, data->isDomain);

  dprintf(1, "64 bytes from %s %s: icmp_seq=%d ttl=%d time=%.*lf ms\n",
          data->reverseDns, ip, stats.pingNb, ttl, precision, data->time);
  free(ip);
}

void print_stats() {
  double loss =
      stats.transmitted == 0
          ? 0
          : ((stats.transmitted - stats.recieved) / stats.transmitted) * 100;
  int loss_precision =
      (stats.transmitted == stats.recieved || stats.transmitted == stats.nbErrs)
          ? 0
          : 3;
  stats.average = find_average();
  double stddev = find_stddev();

  dprintf(1, "--- %s ping statistics ---\n", stats.nameDestination);
  if (stats.nbErrs == 0) {
    dprintf(1, "%d packets transmitted, %d received, %.*lf%% packet loss\n",
            stats.transmitted, stats.recieved, loss_precision, loss);
  } else {
    dprintf(1, "%d packets transmitted, %d received, %.*lf%% packet loss\n",
            stats.transmitted, stats.recieved, loss_precision, loss);
  }
  if (stats.nbErrs != stats.pingNb)
    dprintf(1, "round-trip min/avg/max/stddev = %.3lf/%.3lf/%.3lf/%.3lf ms\n",
            stats.min, stats.average, stats.max, stddev);
  free(stats.median_arr);
  free(stats.data->spacket);
  free(stats.data->strIp);
  free(stats.data->networkIp);

  exit(1);
}

void print_alarm(int signal) {
  (void)signal;

  write(1, "alarm signal\n", 13);
  exit(1);
}

void print_head(t_pingData *data) {
  char *ip = outControlIp(data->strIp, true);

  dprintf(1, "PING %s %s: %d data bytes", stats.nameDestination, ip,
          ICMP_PAYLOAD_SIZE);
  if (data->options & 1) {
    int id = getpid();
    dprintf(1, ", id = 0x%x = %d", id, id);
  }
  dprintf(1, "\n");
  free(ip);
}
