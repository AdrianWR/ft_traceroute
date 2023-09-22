#include "traceroute.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

static t_packet packet = {.payload = PROBE_PACKET};

static struct sockaddr_in probe_addr(t_route *route, unsigned int i) {
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(DEFAULT_UDP_PORT + i);
  addr.sin_addr.s_addr = route->addr_in.sin_addr.s_addr;

  return addr;
}

int send_packet(t_route *route, unsigned int seq, unsigned short ttl) {
  int udp_sockfd;
  struct sockaddr_in dest;

  udp_sockfd = init_udp_socket(ttl);
  dest = probe_addr(route, seq - 1);
  if (sendto(udp_sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&dest,
             sizeof(dest)) == 0 < 0) {
    return -1;
  }

  close(udp_sockfd);
  return 0;
}
