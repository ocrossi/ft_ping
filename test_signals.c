#include <signal.h>
#include <stdio.h>

void sighandler(int signum, void *ptr) {
  // Cast the pointer to the expected type
  char *message = (char *)ptr;

  // Print the received message
  printf("Received message: %s\n", message);
}

int main() {
  // Install the signal handler
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigaction(SIGINT, &act, NULL);

  // Send the SIGINT signal with a message
  char message[] = "Hello from SIGINT!";
  raise(SIGINT, message);

  // Keep the program running until interrupted again
  while (1) {
    // Do some work here
  }

  return 0;
}

