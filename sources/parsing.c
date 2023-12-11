#include "../includes/ft_ping.h"
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isNum(char *arg) {
	for (int i = 0; i < ft_strlen(arg); i++) {
		if (arg[i] < '0' || arg[i] > '9')
			return -1;
	}
	return 1;
}

int check_number_next_arg(char *flag, char *arg) {
	int c_res = -1;
	if (arg == NULL || ft_strlen(arg) == 0 || isNum(arg) == -1) {
		fprintf(stderr, "%s flag requires a signed number\n", flag);
		print_usage(0);
		return -1;
	} else {
		c_res = ft_atoi(arg);
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
		for (int j = 0; j < ft_strlen(acceptedFlags); j++) {
			if (c == acceptedFlags[j]) {
				data->options += 1 << j;
				ret = true;
				if (c == 'c' || c == 't' || c == 'W')
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
			if (ft_strlen(args[i]) == 1) {
				print_usage(0);
			}
			if ((indexBadFlag = check_managed_flags(args[i], data)) != 0) {
				print_usage(args[i][indexBadFlag]);
			}
			isDestination = false;
		}
		if (data->options & 4) { //check if c is lit
			if (i == ac - 1) {
				print_usage(0);
			}
			data->max_ping = check_number_next_arg(args[i], args[i + 1]);
			if (data->max_ping <= 0 || data->max_ping >= MAX_INT) {
				printf("ping: invalid argument: '%d': out of range: 1 <= value <= %d\n", 
							data->max_ping, MAX_INT);
			}
			data->options ^= 4; //deactivate c to enter only once
			i++;
			isDestination = false;
		}
		if (data->options & 8) { //check if t is lit
			if (i == ac - 1) {
				print_usage(0);
			}
			data->ttl = check_number_next_arg(args[i], args[i + 1]);
			if (data->ttl <= 0 || data->ttl >= MAXTTL) {
				printf("ping: invalid argument: '%d': out of range: 1 <= value <= %d\n", 
							data->ttl, MAXTTL);
			}
			data->options ^= 8; //deactivate t to enter only once
			i++;
			isDestination = false;
		}
		if (data->options & 16) { //check if W is lit
			if (i == ac - 1) {
				print_usage(0);
			}
			data->timeout = check_number_next_arg(args[i], args[i + 1]);
			if (data->timeout <= 0 || data->timeout >= MAX_INT) {
				printf("ping: invalid argument: '%d': out of range: 1 <= value <= %d\n", 
							data->timeout, MAX_INT);
			}
			data->options ^= 16; //deactivate W to enter only once
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
		fprintf(stderr, "getaddrinfo crashed with %s\n", gai_strerror(ret));
		exit(1);
	}
	h = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
	data->strIp = ft_strdup(buff);
	data->networkIp = h;
}

void parsing(int ac, char **args, t_pingData *data) {
	char flag = 0;
	int index_dest;
	
	ft_memset(data, 0, sizeof(*data));
	if (ac < 2) {
		fprintf(stderr, "ping: usage error: Destination address required\n");
		exit(0);
	}
	index_dest = manage_options(ac, args, data);
	manage_destination(args[index_dest], data);
	stats.nameDestination = args[index_dest];
}
