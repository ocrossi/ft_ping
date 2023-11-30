#include "../includes/ft_ping.h"

int main(int ac, char** av) {
	t_pingData data = parsing(ac, av);
	create_and_send_packet(&data);
	// send_icmp_packet();
	// wait_response();
	// dispay_output();
	
	return 0;
}
