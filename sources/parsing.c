#include "../includes/ft_ping.h"
#include <string.h>

void print_usage(char invalidFlag) {
	if (invalidFlag != 0)
		fprintf(stderr, "ping: invalid option -- '%c'\n\n", invalidFlag);
	fprintf(stderr, "Usage\n\tping [options] <destination>\n\nOptions:\n");
	fprintf(stderr, "\t<destination>\t\t dns name or ip address");
	fprintf(stderr, "-v\t\t\t\tverbose output");
	exit(1);
}

int isNum(char *arg) {
	for (int i = 0; i < ft_strlen(arg); i++) {
		if (arg[i] < '0' || arg[i] > '9')
			return -1;
	}
	return 1;
}

int check_number_next_arg(char *arg) {
	int c_res = -1;
	if (arg == NULL || ft_strlen(arg) == 0 || isNum(arg) == -1) {
		return -1;
	} else {
		printf("wesh ; arg %s\n", arg);
		c_res = ft_atoi(arg);
	}
	printf("wesh res dans check nb next arg %d\n", c_res);
	
	return c_res;
}

bool check_managed_flag(int indexes[], char **args, t_pingData *data) {
	char c = args[indexes[0]][indexes[1]];

	for (int i = 0; i < ft_strlen(acceptedFlags); i++) {
		if (c == '?') {
			print_usage(0);
		}
		if (c == acceptedFlags[i]) {
			data->options += 1 << i;
			print_memory(&(data->options), 1, 2);
			return true;
		}
	}
	return false;
}

void manage_options(int ac, char **args, t_pingData *data) {
	int c_counter;
	int args_to_remove[ac];
	int j = 0;

	ft_memset(args_to_remove, -1, sizeof(args_to_remove));
	print_memory(args_to_remove, sizeof(args_to_remove), 16);
	for (int i = 1; i < ac; i++) {
				/* dprintf(1, "are we good?"); */
		printf("arg tested = %s\n", args[i]);
		if (args[i][0] == '-') {
			if (ft_strlen(args[i]) == 1) {
				print_usage(0);
			}
			for (int j = 1; j < ft_strlen(args[i]); j++) {
				if (check_managed_flag((int [2]){i, j}, args, data) == false) {
					print_usage(args[i][j]);
				}
			}
			args_to_remove[j++] = i;
		}
		if (i >= 2 && data->options & 4) { //check if c is lit
			printf("-c activated\n");
			if (i == ac - 1) {
				print_usage(0);
			}
			data->max_ping = check_number_next_arg(args[i]);
			printf("arg tested for c value = %s\n", args[i]);
			if (data->max_ping <= 0 || data->max_ping >= MAX_INT) {
				printf("ping: invalid argument: '%d': out of range: 1 <= value <= %d\n", 
							data->max_ping, MAX_INT);
			}
			data->options ^= 4; //deactivate c to enter only once
			args_to_remove[j++] = i;
			// i+=2;
			// printf("max ping for c = %d\n", data->max_ping);
			// printf("i = %d\n", data->max_ping);
		}
	}
	for (int i = 0; i < j; i++) {
		printf("arg to remove to test only destination is %s\n", args[args_to_remove[i]]);
	}
}

void manage_destination(int ac, char **args, t_pingData *data) {
	char buff[INET_ADDRSTRLEN];
	int ret;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct sockaddr_in *h;
	bool control = false;
	ft_memset(&hints, 0, sizeof(hints));
	ft_memset(&buff, 0, sizeof(buff));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = 0;

	for (int i = 1; i < ac; i++) {

		if (args[i][0] == '-') {
			continue;
		}
		if ((ret = getaddrinfo(args[i], NULL, &hints, &res)) != 0) {
			fprintf(stderr, "getaddrinfo crashed with %s\n", gai_strerror(ret));
			exit(1);
		}
		else if (control != true) {
			control = true;
			h = (struct sockaddr_in *)res->ai_addr;
			inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
			data->strIp = strdup(buff);
			data->networkIp = h;
		}
	}
}

t_pingData parsing(int ac, char **args) {
	char flag = 0;
	t_pingData data;
	
	ft_memset(&data, 0, sizeof(data));
	if (ac < 2) {
		fprintf(stderr, "ping: usage error: Destination address required\n");
		exit(0);
	}
	manage_options(ac, args, &data);
	manage_destination(ac, args, &data);
	return data;
}
