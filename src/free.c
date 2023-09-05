#include "traceroute.h"
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

void free_route(t_route *route) {
  freeaddrinfo(route->addrinfo);
  free(route);
}
