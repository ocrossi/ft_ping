#include "../includes/ft_ping.h"

void exit_bad_int(t_pingData *data) {
    fprintf(stderr, "ping: invalid argument: '%d': out of range: 1 <= value <= %d\n",
               data->max_ping, MAX_INT);
    exit(1);
}

int isNum(char *arg) {
  for (int i = 0; i < ft_strlen(arg); i++) {
    if (arg[i] < '0' || arg[i] > '9')
      return -1;
  }
  return 1;
}

int isFloatNum(char *arg) {
  bool dot = false;
  for (int i = 0; i < ft_strlen(arg); i++) {
    if (arg[i] == '.') {
      dot = true;
      continue;
    }
    if (arg[i] < '0' || arg[i] > '9')
      return -1;
  }
  return 1;
}

int check_number_next_arg_int(char *flag, char *arg) {
  int c_res = -1;
  if (arg == NULL || ft_strlen(arg) == 0 || isNum(arg) == -1) {
    fprintf(stderr, "%s flag requires an unsigned number\n", flag);
    print_usage(0);
    return -1;
  } else {
    c_res = ft_atoi(arg);
  }
  return c_res;
}

double check_number_next_arg_float(char *flag, char *arg) {
  double c_res = -1;
  if (arg == NULL || ft_strlen(arg) == 0 || isFloatNum(arg) == -1) {
    fprintf(stderr, "%s flag requires a floating unsigned number\n", flag);
    print_usage(0);
    return -1;
  } else {
    c_res = ft_atolf(arg);
  }
  return c_res;
}

int check_managed_flags(char *arg, t_pingData *data) {
  int incomp = 0;
  bool ret = false;
  char c;
  int len = ft_strlen(arg);

  for (int i = 1; i < len; i++) {
    c = arg[i];
    if (c == '?') {
      print_usage(0);
    }
    for (int j = 0; j < OPT_NB; j++) {
      if (c == data->acceptedFlags[j]) {
        data->options += 1 << j;
        ret = true;
        if (c == 'c' || c == 't' || c == 'i')
          incomp++;
      }
      if (incomp > 1)
        return i;
    }
  }
  if (ret == false)
    return -1;
  return 0;
}

int manage_options(int ac, char **args, t_pingData *data) {
  int destCount = 0;
  int index_destination = 0;
  int indexBadFlag;
  bool isDestination;

  for (int i = 1; i < ac; i++) {
    isDestination = true;
    if (args[i][0] == '-') {
      // arg is just -
      if (ft_strlen(args[i]) == 1) {
        print_usage(0);
      }
      // arg isnt in managed flags list
      if ((indexBadFlag = check_managed_flags(args[i], data)) != 0) {
        print_usage(args[i][indexBadFlag]);
      }
      isDestination = false;
    }
    if (data->options & 4) { // check if c is lit
      if (i == ac - 1) {
        print_usage(0);
      }
      data->max_ping = check_number_next_arg_int(args[i], args[i + 1]);
      if (data->max_ping <= 0 || data->max_ping >= MAX_INT) {
        exit_bad_int(data);
      }
      data->options ^= 4; // deactivate c to enter only once
      i++;
      isDestination = false;
    }
    if (data->options & 8) { // check if t is lit
      if (i == ac - 1) {
        print_usage(0);
      }
      data->ttl = check_number_next_arg_int(args[i], args[i + 1]);
      if (data->ttl <= 0 || data->ttl >= MAXTTL) {
        exit_bad_int(data);
      }
      data->options ^= 8; // deactivate t to enter only once
      i++;
      isDestination = false;
    }
    if (data->options & 16) { // check if i is lit
      if (i == ac - 1) {
        print_usage(0);
      }
      double res = check_number_next_arg_float(args[i], args[i + 1]);
      data->interval = convert_to_microseconds(res);
      data->intervalStr = args[i + 1];
      if (data->interval <= 0 || data->interval >= MAX_INT) {
        printf("ping: invalid argument: '%u': out of range: 0 < value <= %d\n",
               data->interval, MAX_INT);
        exit(1);
      }
      data->options ^= 16; // deactivate i to enter only once
      i++;
      isDestination = false;
    }
    if (isDestination == true) {
      index_destination = i;
      destCount++;
    }
  }
  if (destCount > 1) {
    fprintf(stderr, "ping: there are more than 1 destination, exiting ...\n\n");
    print_usage(0);
  } else if (destCount == 0) {
    fprintf(stderr, "ping: no destination, exiting ...\n\n");
    print_usage(0);
  }
  return index_destination;
}

void manage_destination(char *arg, t_pingData *data) {
  char buff[INET_ADDRSTRLEN];
  int ret;
  struct addrinfo hints;
  struct addrinfo *res, *p;
  struct sockaddr_in *h;
  ft_memset(&hints, 0, sizeof(hints));
  ft_memset(&buff, 0, sizeof(buff));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_RAW;
  hints.ai_flags = 0;

  if ((ret = getaddrinfo(arg, NULL, &hints, &res)) != 0) {
    fprintf(stderr, "ping: %s: %s\n", arg, gai_strerror(ret));
    exit(1);
  }
  h = (struct sockaddr_in *)res->ai_addr;
  inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
  data->strIp = ft_strdup(buff);
  data->networkIp = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  ft_memcpy(data->networkIp, h, sizeof(struct sockaddr_in));
  freeaddrinfo(res);
}

bool checkIsDomainName(char *in) {
  struct hostent bufs, *host;
  char buff[256];
  int r;
  int h_errno;

  r = gethostbyname_r(in, &bufs, buff, sizeof(buff), &host, &h_errno);

  if (r == 0) {
    char *ip = inet_ntoa(*(struct in_addr *)host->h_addr_list[0]);
    if (ft_strcmp(ip, in) == 0) {
      return false;
    }
    return true;
  }
  return false;
}

void parsing(int ac, char **args, t_pingData *data) {
  char flag = 0;
  int index_dest;

  if (ac < 2) {
    fprintf(stderr, "ping: usage error: Destination address required\n");
    exit(0);
  }
  strcpy(data->acceptedFlags, "v?ctiq");
  index_dest = manage_options(ac, args, data);
  manage_destination(args[index_dest], data);
  stats.nameDestination = args[index_dest];
  data->isDomain = checkIsDomainName(args[index_dest]);
}
