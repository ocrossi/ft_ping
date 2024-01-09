#include "../includes/ft_ping.h"

int get_milisec_precision(t_pingData *data) {
	int digit = 0;
	double time = data->time;


	for(int i = 0; i < 3; i++) {
		if (time / 10 > 0.1) {
			digit++;
			time /= 10;
		}
	}
	return 3 - digit;
}

double sqrtd(double in) {
	double groot = in / 3;

	for (int i = 0; i < 64; i++) {
		groot = (groot + in / groot) / 2;
	}
	return in == 0 ? 0 : groot;
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

useconds_t convert_to_microseconds(double seconds) {
	useconds_t res = seconds * 1000000;

	return res;
}

void reverseDNS(t_pingData *data) {
	struct sockaddr_in temp;
	socklen_t len;
	char buff[NI_MAXHOST];

	
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = data->rpacket->ipHeader.saddr;
	len = sizeof(temp);

	if (!getnameinfo((struct sockaddr*)&temp, len, buff, sizeof(buff), NULL, 0, NI_NAMEREQD)) {
		data->reverseDns = ft_strdup(buff);
	}
}

char *get_ip_reverseDNS(t_pingData *data) {
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

	if ((ret = getaddrinfo(data->reverseDns, NULL, &hints, &res)) != 0) {
		struct in_addr ip_source;
		ip_source.s_addr = data->rpacket->ipHeader.saddr;
		inet_ntop(AF_INET, &ip_source, buff, INET_ADDRSTRLEN);
	} else {
		h = (struct sockaddr_in *)res->ai_addr;
		inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
	}
	return ft_strdup(buff);
} 
