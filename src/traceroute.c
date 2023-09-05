#include "traceroute.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int traceroute(t_route *route) {
  signal(SIGINT, interrupt_handler);

  if (init_socket(route) != 0) {
    return 1;
  }

  if (address_lookup(route) != 0) {
    fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s\'\n", route->host);
    return 1;
  }

  // Send UDP packets with TTL = 1.
  int ttl = 1;
  // get localhost address in network byte order
  struct sockaddr_in localhost;
  localhost.sin_family = AF_INET;
  localhost.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  localhost.sin_port = htons(33434);
  int src = localhost.sin_addr.s_addr;

  // get the destination address in network byte order
  int dst = ((struct sockaddr_in *)route->addrinfo->ai_addr)->sin_addr.s_addr;
  t_packet packet = new_packet(ttl, src, dst);

  printf("Addrinfo ai_family: %d\n", route->addrinfo->ai_family);
  printf("Addrinfo ai_socktype: %d\n", route->addrinfo->ai_socktype);
  printf("Addrinfo ai_protocol: %d\n", route->addrinfo->ai_protocol);
  printf("Addrinfo ai_addrlen: %d\n", route->addrinfo->ai_addrlen);
  printf("Addrinfo ai_addr: %s\n",
         inet_ntoa(((struct sockaddr_in *)route->addrinfo->ai_addr)->sin_addr));
  printf("Addrinfo ai_canonname: %s\n", route->addrinfo->ai_canonname);
  printf("Addrinfo ai_next: %p\n", route->addrinfo->ai_next);

  // send the packet
  if (sendto(route->sockfd, &packet, sizeof(packet), 0,
             route->addrinfo->ai_addr, route->addrinfo->ai_addrlen) < 0) {
    fprintf(stderr, "sendto error: %s\n", strerror(errno));
    return 1;
  }

  printf("traceroute to %s (%s), 30 hops max, 60 byte packets\n", route->host,
         route->addr);

  return 0;
}
