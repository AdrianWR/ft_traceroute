#include "traceroute.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int address_lookup(t_route *route) {
  int status;
  struct addrinfo hints;
  struct addrinfo *res;
  int ai_family;

  hints.ai_family = AF_INET;
  hints.ai_socktype = 0;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_CANONNAME;

  if ((status = getaddrinfo(route->host, NULL, &hints, &res))) {
    fprintf(stderr, "%s: %s\n", route->host, gai_strerror(status));
    return (1);
  }
  route->addr_in = *(struct sockaddr_in *)(res->ai_addr);
  ai_family = res->ai_family;
  freeaddrinfo(res);

  if (ai_family != AF_INET) {
    fprintf(stderr, "traceroute: address family not supported\n");
    return (1);
  }

  return (0);
}

const char *reverse_dns_lookup(struct sockaddr_in *addr, socklen_t addrlen) {
  static char host[NI_MAXHOST];
  int flags = 0;

  if (getnameinfo((struct sockaddr *)addr, addrlen, host, NI_MAXHOST, NULL, 0,
                  flags) == 0) {
    return (host);
  }
  return (NULL);
}
