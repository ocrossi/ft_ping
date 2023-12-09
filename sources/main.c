#include "../includes/ft_ping.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>

char acceptedFlags[] = "v?ctW";
t_statData stats;



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
		// printf("in find average %d %lf\n", i, avg);
		// printf("in find average %d %lf\n", i, stats.median_arr[i]);
		avg += stats.median_arr[i];
		i++;
	}

/* 	printf("in find average %lf\n", avg); */

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

void print_stats(int signum) {
	double loss = ((stats.transmitted - stats.recieved) / stats.transmitted) * 100;
	stats.average = find_average();
	printf("%av lf\n", stats.average);
	double stddev = find_stddev();
	double time = convert_to_milliseconds(stats.data->recieveTime, stats.data->start_time); 

	printf("--- %s ping statistics ---\n", stats.nameDestination);
	printf("%d packets transmitted, %d received, %lf%% packet loss, time %.0lfms\n", stats.transmitted, stats.recieved, loss, time);
	printf("rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", stats.min, stats.average, stats.max, stddev);
	exit(1);
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

void manage_time(t_pingData *data) {
	// double fMicro = (data->recieveTime.tv_usec - data->sendTime.tv_usec) + (data->recieveTime.tv_sec - data->sendTime.tv_sec) * 1000000;
	// double fMilli = fMicro / 1000;
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

int main(int ac, char** av) {
	t_pingData data;
	int sockFd;

	ft_memset(&stats, 0, sizeof(stats));
	ft_memset(&data, 0, sizeof(data));
	parsing(ac, av, &data);
	signal(SIGINT, &print_stats);
	sockFd = create_socket(&data);
	construct_packet(&data);
	stats.median_arr = (double *)malloc(sizeof(double) * MAX_ARR_MEDIAN);
	stats.data = &data;
	set_median_arr();
	gettimeofday(&data.start_time, NULL);
	while (1) {
		if (data.max_ping != 0 && stats.pingNb == data.max_ping) {
			print_stats(0);
			exit(0);
		}
		stats.pingNb++;
		send_packet(&data, sockFd);
		recieve_packet(&data, sockFd);
		manage_time(&data);
		reverseDNS(&data);
		print_output(&data);
		usleep(ONE_SEC);
	}
	return 0;
}
