#include "traceroute.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static size_t packet_size =
    sizeof(struct iphdr) + sizeof(struct icmphdr) + PROBE_PACKET_SIZE;

static int fetch_packet(t_hop hop, unsigned int *got_there,
                        unsigned int *unreachable) {
  if (hop.ihp.icmp_type == ICMP_TIME_EXCEEDED &&
      hop.ihp.icmp_code == ICMP_TIMXCEED_INTRANS)
    return 1;

  switch (hop.ihp.icmp_code) {
  case ICMP_UNREACH_PORT:
    (*got_there)++;
    return 1;
  case ICMP_UNREACH_NET:
    (*unreachable)++;
    printf("!N ");
    return 1;
  case ICMP_UNREACH_HOST:
    (*unreachable)++;
    printf("!H ");
    return 1;
  case ICMP_UNREACH_PROTOCOL:
    (*got_there)++;
    printf("!P ");
    return 1;
  case ICMP_UNREACH_NEEDFRAG:
    (*unreachable)++;
    printf("!F ");
    return 1;
  case ICMP_UNREACH_SRCFAIL:
    (*unreachable)++;
    printf("!S ");
    return 1;
  }

  return 0;
}

int traceroute(t_route *route) {
  int max_ttl = DEFAULT_MAX_HOPS;
  unsigned int seq = 0;
  unsigned int nprobes = 3;
  unsigned long last_addr = 0;
  unsigned int got_there = 0;
  unsigned int unreachable = 0;

  signal(SIGINT, interrupt_handler);

  if (init_icmp_socket(route) != 0) {
    return 1;
  }

  if (address_lookup(route) != 0) {
    fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s\'\n", route->host);
    return 1;
  }

  printf("traceroute to %s (%s), %u hops max, %zu byte packets\n", route->host,
         route->addr, max_ttl, packet_size);
  for (int ttl = 1; ttl <= max_ttl; ++ttl) {
    struct timeval t1, t2;
    int cc;
    t_hop hop;

    printf("%2d ", ttl);
    for (unsigned int probe = 0; probe < nprobes; probe++) {
      gettimeofday(&t1, NULL);
      send_packet(route, ++seq, ttl);
      while ((cc = receive_packet(route, &hop) > 0)) {
        gettimeofday(&t2, NULL);
        if (hop.from.sin_addr.s_addr != last_addr) {
          printf("%s (%s) ", inet_ntoa(hop.from.sin_addr),
                 inet_ntoa(hop.from.sin_addr));
          last_addr = hop.from.sin_addr.s_addr;
        }
        printf("%.3f ms ", time_diff_ms(&t1, &t2));
        fetch_packet(hop, &got_there, &unreachable);
        break;
      }
      if (cc == 0)
        printf("* ");
    }
    printf("\n");
    if (got_there || unreachable >= nprobes - 1)
      exit(0);
  }

  return 0;
}
