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

static size_t packet_size =
    sizeof(struct iphdr) + sizeof(struct icmphdr) + PROBE_PACKET_SIZE;

int traceroute(t_route *route) {
  unsigned int hops;

  signal(SIGINT, interrupt_handler);

  hops = DEFAULT_MAX_HOPS;
  route->udp_port = DEFAULT_UDP_PORT;

  if (init_icmp_socket(route) != 0) {
    return 1;
  }

  if (address_lookup(route) != 0) {
    fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s\'\n", route->host);
    return 1;
  }

  printf("traceroute to %s (%s), %u hops max, %zu byte packets", route->host,
         route->addr, hops, packet_size);
  for (unsigned int i = 0; i < hops; i++) {
    send_packet(route, i);
    receive_packets(route, i);
  }

  return 0;
}
