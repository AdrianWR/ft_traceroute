#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void interrupt_handler(int sig) {

  (void)sig;

  if (sig == SIGINT) {
    printf("\n");
  }

  exit(sig);
}
