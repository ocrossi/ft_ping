#include "../includes/ft_ping.h"

void print_usage(char invalidFlag) {
	if (invalidFlag != 0)
		fprintf(stderr, "ping: invalid option -- '%c'\n\n", invalidFlag);
	fprintf(stderr, "Usage\n\tping [options] <destination>\n\nOptions:\n");
	fprintf(stderr, "\t<destination>\t\t dns name or ip address");
	fprintf(stderr, "-v\t\t\t\tverbose output");
	exit(1);
}

bool check_managed_flag(char c, t_pingData *data) {
	char acceptedFlags[] = "v?";

	for (int i = 0; i < ft_strlen(acceptedFlags); i++) {
		if (c == acceptedFlags[i]) {
			data->options[i] = true;
			printf("we set up the %c flag\n", c);
			return true;
		}
	}
	return false;
}

void manage_options(int ac, char **args, t_pingData *data) {
	for (int i = 1; i < ac; i++) {
		if (args[i][0] == '-') {
			if (ft_strlen(args[i]) == 1) {
				print_usage(0);
			}
			for (int j = 1; j < ft_strlen(args[i]); j++) {
				if (check_managed_flag(args[i][j], data) == false) {
					print_usage(args[i][j]);
				}
			}
		}
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
			data->strIp = buff;
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
