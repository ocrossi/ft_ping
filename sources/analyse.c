#include "../includes/ft_ping.h"

void manage_time(t_pingData *data) {
	double fMilli = convert_to_milliseconds(data->recieveTime, data->sendTime);
	int index_median_arr = (stats.recieved - 1 == -1) ? 0: stats.recieved - 1;


	if (stats.pingNb == 0) {
		stats.min =  fMilli;
	}

	data->time = fMilli;
	stats.max = (stats.max > fMilli) ? stats.max : fMilli;
	stats.min = (stats.min < fMilli) ? stats.min : fMilli;
	stats.median_arr[index_median_arr] = fMilli;
}

void check_icmp_fragment(t_pingData *data) {
  if (data->retPrintSize > (int)(PACKET_SIZE + sizeof(struct iphdr) + 8)) { //??? the multi part extension... final 8 buytes len of icmp error message
	  data->retPrintSize -= 32;
  }
	data->retPrintSize -= sizeof(struct iphdr);
}

bool check_packet_data(t_pingData *data) {
	int coderouge = data->rpacket->icmpHeader.type;

	if (coderouge == 0 || coderouge == 8) {
		return true;
	} else {
		switch (coderouge) {
			case 3:
				data->error = EICMP_DEST_UNREACH;
				break;
			case 4:
				data->error = EICMP_SOURCE_QUENCH;
				break;
			case 5:
				data->error = EICMP_REDIRECT;
				break;
			case 11:
				data->error = EICMP_TIME_EXCEEDED;
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
		check_icmp_fragment(data);
		stats.nbErrs++;
		return false;
	}
}
