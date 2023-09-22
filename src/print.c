#include "traceroute.h"
#include <netinet/in.h>

// static float get_elapsed_time(t_hop hop) {
//  return time_diff_ms(&hop.start, &hop.end);
//}

// void print_hop(t_hop *hop) {
//  static char last_addr[INET_ADDRSTRLEN] = {0};
//  static unsigned short last_ttl = 0;
//
//  if (hop->ttl != last_ttl) {
//    printf("\n%2d ", hop->ttl);
//    last_ttl = hop->ttl;
//  }
//
//  if (!hop->received) {
//    printf(" *");
//  } else {
//    if (ft_strncmp(last_addr, hop->addr, INET_ADDRSTRLEN) != 0) {
//      printf(" %s", hop->addr);
//      ft_memcpy(last_addr, hop->addr, INET_ADDRSTRLEN);
//    }
//    printf(" (%.3f ms)", get_elapsed_time(*hop));
//  }
//}
