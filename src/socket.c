#include "traceroute.h"
#include <errno.h>
#include <string.h>

int init_socket(t_route *route) {
  int sockfd;

  // Create IPv4 socket for sending raw datagrams
  if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    fprintf(stderr, "traceroute: socket: %s\n", strerror(errno));
    return 1;
  }

  // Enable sending broadcast datagrams
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &(int){1}, sizeof(int)) <
      0) {
    fprintf(stderr, "traceroute: setsockopt: %s\n", strerror(errno));
    return 1;
  }

  // Enable ip header include
  if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &(int){1}, sizeof(int)) < 0) {
    fprintf(stderr, "traceroute: setsockopt: %s\n", strerror(errno));
    return 1;
  }

  // Bind socket to localhost
  struct sockaddr_in localhost;
  localhost.sin_family = AF_INET;
  localhost.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  localhost.sin_port = htons(33434);
  if (bind(sockfd, (struct sockaddr *)&localhost, sizeof(localhost)) < 0) {
    fprintf(stderr, "traceroute: bind: %s\n", strerror(errno));
    return 1;
  }

  route->sockfd = sockfd;
  return 0;
}
