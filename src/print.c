#include "traceroute.h"

static float get_elapsed_time(t_hop hop) {
  return time_diff_ms(&hop.start, &hop.end);
}

void print_hops(t_route *route, t_hop *hops) {
  t_hop *hop;
  char first_addr[INET_ADDRSTRLEN];

  printf("%2d ", route->ttl);
  for (unsigned int i = 0; i < 3; i++) {
    hop = &hops[i];

    if (!hop->received) {
      printf(" *");
    } else {
      if (i == 0) {
        ft_memcpy(first_addr, hop->addr, sizeof(first_addr));
        printf(" %s (%s)", hop->addr, hop->addr);
      }
      if (ft_strncmp(first_addr, hop->addr, sizeof(first_addr)) == 0) {
        printf("  %.3f ms", get_elapsed_time(*hop));
      }
    }
  }
  printf("\n");
}
