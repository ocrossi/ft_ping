#include "../includes/ft_ping.h"

char acceptedFlags[] = "v?c";

int main(int ac, char** av) {
	t_pingData data = parsing(ac, av);
	int sockFd;
	char *rpack;

	sockFd = create_packet(&data);
	// while (1) {
	// 	if (data.pingNb == data.max_ping) 
	// 		break;
	// 	data.pingNb++; // might be the only thing to change when sending multiple packets
	// 	send_packet(&data, sockFd);
	// 	rpack = recieve_packet(&data, sockFd);
	// 	print_output(&data, rpack);
	// 	usleep(ONE_SEC);
	// }
	//display stats?
	return 0;
}
