#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

// deja on test sans exit pr voir
void print_stats() {
  dprintf(1, "1\n");
  dprintf(1, "2\n");
  dprintf(1, "3\n");
  dprintf(1, "4\n");
  dprintf(1, "5\n");
  dprintf(1, "6\n");
  dprintf(1, "7\n");
  dprintf(1, "8\n");
  dprintf(1, "9\n");
  dprintf(1, "10\n");
  dprintf(1, "11\n");
  dprintf(1, "12\n");
  dprintf(1, "13\n");
  dprintf(1, "14\n");
  dprintf(1, "15\n");
  dprintf(1, "16\n");
  dprintf(1, "17\n");
  dprintf(1, "18\n");
  dprintf(1, "19\n");
  dprintf(1, "20\n");
  exit(0);
}

void sigint_handler(int signal) {
  if (signal == SIGALRM) {
    printf("got sigalrm\n");
  }
  if (signal == SIGINT) {
    printf("got sigint\n");
    print_stats();
  }
}

void set_signal_action() {
  struct sigaction act;

  bzero(&act, sizeof(struct sigaction));

  act.sa_handler = sigint_handler;

  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  if (sigaction(SIGINT, &act, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
}

int main() {
  set_signal_action();

  while (1) {
    sleep(1);
    printf("waiting ...\n");
  }
}
