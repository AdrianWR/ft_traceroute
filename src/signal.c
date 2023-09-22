#include "traceroute.h"
#include <signal.h>
#include <stdlib.h>

void interrupt_handler(int sig) {

  switch (sig) {
  case SIGINT:
    free(g_route);
    exit(0);
  case SIGQUIT:
    exit(0);
  }
}
