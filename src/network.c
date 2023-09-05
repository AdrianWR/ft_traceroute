#include "traceroute.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int address_lookup(t_route *route) {
  int status;
  struct addrinfo hints;
  struct sockaddr_in *ipv4;
  char *addr;

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_flags = AI_CANONNAME;

  if ((status = getaddrinfo(route->host, NULL, &hints, &route->addrinfo))) {
    fprintf(stderr, "%s: %s\n", route->host, gai_strerror(status));
    return 1;
  }

  if (route->addrinfo->ai_family == AF_INET) {
    ipv4 = (struct sockaddr_in *)(route->addrinfo)->ai_addr;
    addr = inet_ntoa(ipv4->sin_addr);
    *(char *)ft_mempcpy(route->addr, addr, ft_strlen(addr)) = '\0';
  } else {
    fprintf(stderr, "traceroute: address family not supported\n");
    return 1;
  }

  return 0;
}
