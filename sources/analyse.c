#include "../includes/ft_ping.h"
#include <netinet/ip.h>

void manage_time(t_pingData *data) {
	double fMilli = convert_to_milliseconds(data->recieveTime, data->sendTime);
	int index_median_arr = (stats.recieved - 1 == -1) ? 0: stats.recieved - 1;


	if (stats.pingNb == 1) {
		stats.min =  fMilli;
	}

	data->time = fMilli;
	stats.max = (stats.max > fMilli) ? stats.max : fMilli;
	stats.min = (stats.min < fMilli) ? stats.min : fMilli;
	stats.median_arr[index_median_arr] = fMilli;
}

// void check_icmp_fragment(t_pingData *data, int code) {
// 	int err = data->rpacket->icmpHeader.code;
//
// 	printf("code d erreur? %d\n", err);
//
// 	for (int i = 0; i < 4; i++) {
// 		if (data->recievedBytesArray[24 + i] != 0) {
// 			dprintf(1, "code %d\n", code);
// 			if (code == 11 || code == 13) {
// 				data->retPrintSize = sizeof(struct iphdr) * 3 + ICMP_PAYLOAD_SIZE;
// 				dprintf(1, "ALLOO1\n");
// 			} else if (code == 3) {
// 				dprintf(1, "ALLOO2\n");
// 				data->retPrintSize = ICMP_PAYLOAD_SIZE + sizeof(struct iphdr);
// 			}
// 			return;
// 		}
// 	}
// 	dprintf(1, "FRAG\n");
// }

bool check_packet_data(t_pingData *data) {
	int type = data->rpacket->icmpHeader.type; //  cette ligne

	if (type == 0 || type == 8) {
		return true;
	} else {
		switch (type) {
			case 3:
				data->error = EICMP_DEST_UNREACH;
				data->retPrintSize = ICMP_PAYLOAD_SIZE + sizeof(struct iphdr);
				break;
			case 4:
				data->error = EICMP_SOURCE_QUENCH;
				break;
			case 5:
				data->error = EICMP_REDIRECT;
				break;
			case 11:
				data->error = EICMP_TIME_EXCEEDED;
				data->retPrintSize = sizeof(struct iphdr) * 3 + ICMP_PAYLOAD_SIZE;
				break;
			case 12:
				data->error = EICMP_PARAMETERPROB;
				break;
			case 13:
				data->error = EICMP_TIMESTAMP;
				break;
			case 14:
				data->error = EICMP_TIMESTAMPREPLY;
				break;
			case 15:
				data->error = EICMP_INFO_REQUEST;
				break;
			case 16:
				data->error = EICMP_INFO_REPLY;
				break;
			case 17:
				data->error = EICMP_ADDRESS;
				break;
			case 18:
				data->error = EICMP_ADDRESSREPLY;
				break;
			default:
				data->error = UNKNOWN_ERR_CODE;
		}
		printf("error type : %d\n", type);
		printf("error : %s\n", data->error);
		// // if (type != 11) {
		data->retPrintSize -= sizeof(struct ip);
		// }
		// check_icmp_fragment(data, type);
		stats.nbErrs++;
		return false;
	}
}
