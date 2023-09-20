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
  addr.sin_addr.s_addr =
      ((struct sockaddr_in *)route->addrinfo->ai_addr)->sin_addr.s_addr;

  return addr;
}

int send_packet(t_route *route, unsigned int i) {
  t_hop *hop;
  int udp_sockfd;
  struct sockaddr_in dest;
  unsigned short ttl;

  ttl = (i / 3) + 1;
  hop = &route->hops[i];
  hop->sent = false;
  hop->received = false;
  hop->ttl = ttl;

  udp_sockfd = init_udp_socket(ttl);
  dest = probe_addr(route, i);
  if (sendto(udp_sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&dest,
             sizeof(dest)) == 0 < 0) {
    return -1;
  }

  // Save the time when the packet was sent
  hop->sent = true;
  gettimeofday(&hop->start, NULL);

  close(route->udp_sockfd);
  return 0;
}
