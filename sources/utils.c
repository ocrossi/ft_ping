#include "../includes/ft_ping.h"

double sqrtd(double in) {
	double groot = in / 3;

	for (int i = 0; i < 64; i++) {
		groot = (groot + in / groot) / 2;
	}
	return groot;
}

double find_stddev() {
	int i = 0;
	double tempdev = 0;
	double tempdev1= 0;

	while (i < MAX_ARR_MEDIAN && stats.median_arr[i] != -1) {
		for (int j = 0; j < MAX_ARR_MEDIAN && stats.median_arr[j] != -1; j++) {
			tempdev =  stats.median_arr[i] - stats.average;
			tempdev1 += tempdev * tempdev;
		}
		i++;
	}

	return sqrtd(tempdev1 / i);
}

double find_average() {
	int i = 0;
	double avg = 0;

	while (i < MAX_ARR_MEDIAN && stats.median_arr[i] != -1) {
		avg += stats.median_arr[i];
		i++;
	}

	return avg / i;
}

void set_median_arr() {
	double set = -1;

	for (int i = 0; i < MAX_ARR_MEDIAN; i++) {
		stats.median_arr[i] = set;
	}
}


double convert_to_milliseconds(t_val time, t_val base) {
	double fMicro = (time.tv_usec - base.tv_usec) + (time.tv_sec - base.tv_sec) * 1000000;
	double fMilli = fMicro / 1000;

	return fMilli;
}

void reverseDNS(t_pingData *data) {
	struct sockaddr_in temp;
	socklen_t len;
	char buff[NI_MAXHOST];

	
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = data->rpacket->ipHeader.saddr;
	len = sizeof(temp);
	if (getnameinfo((struct sockaddr*)&temp, len, buff, sizeof(buff), NULL, 0, NI_NAMEREQD)) {
		printf("DNS error\n");
	} else {
		data->reverseDns = ft_strdup(buff);
	}
}
