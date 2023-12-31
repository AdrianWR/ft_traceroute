/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroque <aroque@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 15:23:45 by aroque            #+#    #+#             */
/*   Updated: 2023/09/23 15:42:16 by aroque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "traceroute.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

ssize_t receive_packet(t_route *route, t_hop *hop) {
  int retval;
  fd_set readfds;
  struct timeval timeout;
  char buf[512];
  ssize_t cc = 0;

  ft_memset(buf, 0, sizeof(buf));
  FD_ZERO(&readfds);
  FD_SET(route->icmp_sockfd, &readfds);

  hop->fromlen = sizeof(hop->from);
  timeout.tv_sec = route->waittime;
  timeout.tv_usec = 0;

  retval = select(route->icmp_sockfd + 1, &readfds, NULL, NULL, &timeout);
  if (retval == -1) {
    fprintf(stderr, "select error: %s\n", strerror(errno));
    return -1;
  } else if (retval > 0) {
    cc = recvfrom(route->icmp_sockfd, buf, sizeof(buf), 0,
                  (struct sockaddr *)(&hop->from), &hop->fromlen);
    if (cc < 0) {
      fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
      return 1;
    }

    ft_memcpy(&hop->ihp, buf + sizeof(struct iphdr), sizeof(struct icmphdr));
  }

  return (cc);
}
