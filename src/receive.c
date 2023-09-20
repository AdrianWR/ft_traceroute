#include "traceroute.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

static int get_icmp_type(char *packet) {
  struct icmphdr *icmp_header;

  icmp_header = (struct icmphdr *)(packet + sizeof(struct iphdr));

  return icmp_header->type;
}

static unsigned int get_destination_port(char *packet) {
  unsigned char *payload;
  struct udphdr *udp_header;

  payload =
      (unsigned char *)(packet + sizeof(struct iphdr) + sizeof(struct icmphdr));
  udp_header = (struct udphdr *)(payload + sizeof(struct iphdr));

  return ntohs(udp_header->dest);
}

static t_hop trace_packet(t_route *route, char *packet) {
  t_hop *hop;
  unsigned int dest_port;
  struct iphdr *ip_header;
  struct in_addr in;

  // Get the destination hop from the packet
  dest_port = get_destination_port(packet);
  hop = &route->hops[PORT_INDEX(dest_port)];

  // Save the time when the packet was received
  gettimeofday(&hop->end, NULL);

  // Get the IP address of the hop with inet_ntoa
  ip_header = (struct iphdr *)packet;
  in.s_addr = ip_header->saddr;
  ft_memcpy(hop->addr, inet_ntoa(in), sizeof(hop->addr));

  // Mark the hop as received
  hop->received = true;

  return *hop;
}

int receive_packets(t_route *route, unsigned int i) {
  int retval;
  fd_set readfds;
  struct timeval timeout;
  char packet[2048];
  t_hop hop;
  int icmp_type;

  icmp_type = 0;
  FD_ZERO(&readfds);
  FD_SET(route->icmp_sockfd, &readfds);

  timeout.tv_sec = DEFAULT_TIMEOUT;
  timeout.tv_usec = 0;

  retval = select(route->icmp_sockfd + 1, &readfds, NULL, NULL, &timeout);
  if (retval == -1) {
    fprintf(stderr, "select error: %s\n", strerror(errno));
    return 1;
  } else if (retval) {
    if (recvfrom(route->icmp_sockfd, packet, sizeof(packet), 0, NULL, NULL) <
        0) {
      fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
      return 1;
    }
    icmp_type = get_icmp_type(packet);
    hop = trace_packet(route, packet);
  } else {
    // Timeout
    hop = route->hops[i];
    hop.received = false;
  }

  print_hop(&hop);
  if (icmp_type == ICMP_DEST_UNREACH) {
    return 1;
  }
  return 0;
}
