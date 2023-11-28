#include "ft_ping.h"

/* 
 * ici on va checker pour chars . ==> ipv4 chars : ==> ipv6
 *
 * */
// void parse_hostname(char *hname) {
// 	if ()
// 		parse_hostname_IPv4(hname);
//
// 	
// }
//

void print_usage(char invalidFlag) {
	if (invalidFlag != 0)
		printf("ping: invalid option -- '%c'\n\n", invalidFlag);
	
	printf("Usage\n\tping [options] <destination>\n\nOptions:\n");
	printf("\t<destination>\t\t dns name or ip address");
	printf("-v\t\t\t\tverbose output");
}

bool check_managed_flag(char c) {
	char acceptedFlags[] = "v?";

	for (int i = 0; i < ft_strlen(acceptedFlags); i++) {
		if (c == acceptedFlags[i]) {
			return true;
		}
	}
	exit(0);
}

void manage_options(int ac, char **args) {
	for (int i = 1; i < ac; i++) {
		if (args[i][0] == '-') {
			if (ft_strlen(args[i]) == 1) {
				print_usage(0);
				exit(0);
			}
			for (int j = 1; j < ft_strlen(args[i]); j++) {
				if (check_managed_flag(args[i][j]) == false) {
					print_usage(args[i][j]);
					exit(0);
				}
			}
		}
	}
}

void manage_destination(int ac, char **args) {
	unsigned char ip[4];

	for (int i = 1; i < ac; i++) {
		if (args[i][0] == '-') {
			continue;
		}
		// how to know if a string is a dns or ip address in c
		// ok seems to be a compination of etiher getaddressinfo or socket() with
		// socketopts and errno + strerr to manage errors
		int res = inet_pton(AF_INET, args[i], ip);

		if (res == 0) {
			printf("ip address <%s> is not valid\n", args[i]);
			exit(1);
		} else {
			for (int i = 0; i < 4; i++) {
				printf("%02x.", ip[i]);
			}
		}
		// https://www.geeksforgeeks.org/socket-programming-cc/
	}
}

void parsing(int ac, char **args) {
	char flag = 0;

	if (ac < 2) {
		printf("ping: usage error: Destination address required\n");
		exit(0);
	}
	manage_options(ac, args);
	manage_destination(ac, args);
}

int main(int ac, char** av) {
	// t_pindData pingData;
	parsing(ac, av);

	// create_icmp_packet();
	// send_icmp_packet();
	// wait_response();
	// dispay_output();
	
	return 0;
}
