#include "traceroute.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

static t_packet packet = {.payload = PROBE_PACKET};

static struct sockaddr_in probe_addr(t_route *route) {
  struct sockaddr_in addr;
  static unsigned int seq = 0;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(route->udp_port + seq++);
  addr.sin_addr.s_addr = route->addr_in.sin_addr.s_addr;

  return (addr);
}

int send_packet(t_route *route, unsigned short ttl) {
  int udp_sockfd;
  struct sockaddr_in dest;

  udp_sockfd = init_udp_socket(ttl);
  dest = probe_addr(route);
  if (sendto(udp_sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&dest,
             sizeof(dest)) == 0 < 0) {
    return (-1);
  }

  close(udp_sockfd);
  return (0);
}
