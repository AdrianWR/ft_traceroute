#include "traceroute.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

static t_packet packet = {.payload = PROBE_PACKET};

static struct sockaddr_in probe_addr(t_route *route) {
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(route->udp_port);
  addr.sin_addr.s_addr =
      ((struct sockaddr_in *)route->addrinfo->ai_addr)->sin_addr.s_addr;

  return addr;
}

int send_packets(t_route *route) {
  struct sockaddr_in dest;
  struct timeval start;
  t_hop *hop;

  for (int i = 0; i < 3; i++) {
    hop = &route->hops[PORT_INDEX(route->udp_port)];
    hop->sent = false;

    init_udp_socket(route);
    dest = probe_addr(route);
    if (sendto(route->udp_sockfd, &packet, sizeof(packet), 0,
               (struct sockaddr *)&dest, sizeof(dest)) == 0) {
      hop->sent = true;
    }

    // Save the time when the packet was sent
    gettimeofday(&start, NULL);
    hop->start = start;

    route->udp_port++;
    close(route->udp_sockfd);
  }

  return 0;
}
