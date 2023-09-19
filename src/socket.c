#include "traceroute.h"
#include <errno.h>
#include <string.h>

static int bind_udp_socket(int udp_sockfd) {
  struct sockaddr_in s;

  s.sin_family = AF_INET;
  s.sin_port = htons(0);
  s.sin_addr.s_addr = INADDR_ANY;
  if (bind(udp_sockfd, (struct sockaddr *)&s, sizeof(s)) < 0) {
    fprintf(stderr, "bind error: %s\n", strerror(errno));
    return 1;
  }
  return 0;
}

int init_udp_socket(t_route *route) {
  int udp_sockfd;
  int optval = 1;

  // Create IPv4 socket to send UDP packets
  if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0)) < 0) {
    fprintf(stderr, "traceroute: udp socket: %s\n", strerror(errno));
    return 1;
  }
  // Set REUSEADDR and REUSEPORT
  if (setsockopt(udp_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                 sizeof(optval)) < 0) {
    fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
    return 1;
  }

  if (setsockopt(udp_sockfd, SOL_SOCKET, SO_REUSEPORT, &optval,
                 sizeof(optval)) < 0) {
    fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
    return 1;
  }

  if (setsockopt(udp_sockfd, IPPROTO_IP, IP_TTL, &route->ttl,
                 sizeof(route->ttl)) < 0) {
    fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
    return 1;
  }

  if (bind_udp_socket(udp_sockfd) != 0) {
    return 1;
  }

  route->udp_sockfd = udp_sockfd;
  return 0;
}

int init_icmp_socket(t_route *route) {
  int icmp_sockfd;

  // Create IPv4 socket to receive ICMP packets
  if ((icmp_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
    fprintf(stderr, "traceroute: icmp socket: %s\n", strerror(errno));
    return 1;
  }

  route->icmp_sockfd = icmp_sockfd;
  return 0;
}
