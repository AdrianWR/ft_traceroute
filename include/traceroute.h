#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

// Main binary parsing options
#define HELP_OPTION 1

#define PROBE_PACKET "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
#define PROBE_PACKET_SIZE 32
#define PROBE_PACKET_COUNT 3
#define PROBE_MAX_PACKETS 90

#define DEFAULT_TTL 64
#define DEFAULT_MAX_HOPS 30
#define DEFAULT_NPROBES 3
#define DEFAULT_TIMEOUT 1
#define DEFAULT_UDP_PORT 33434
#define DEFAULT_UDP_PAYLOAD_SIZE 32
#define DEFAULT_UDP_PACKET_SIZE 60

typedef struct s_hop {
  struct sockaddr_in from;
  socklen_t fromlen;

  struct icmp ihp;
  struct timeval t1;
  struct timeval t2;
} t_hop;

typedef struct s_route {
  const char *host;
  unsigned int options;
  struct sockaddr_in addr_in;
  int icmp_sockfd;

  unsigned short max_ttl;
  unsigned int nprobes;
} t_route;

typedef struct s_packet {
  char payload[PROBE_PACKET_SIZE];
} t_packet;

extern t_route *g_route;

int traceroute(t_route *route);
int address_lookup(t_route *route);
const char *reverse_dns_lookup(struct sockaddr_in *addr_in, socklen_t fromlen);
int init_icmp_socket(t_route *route);
int init_udp_socket(unsigned short ttl);
t_packet new_packet();
void interrupt_handler(int sig);
void free_route(t_route *route);
int send_packet(t_route *route, unsigned int seq, unsigned short ttl);
ssize_t receive_packet(t_route *route, t_hop *hop);
float time_diff_ms(struct timeval *start, struct timeval *end);

size_t ft_strlen(const char *s);
void *ft_memset(void *b, int c, size_t len);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void *ft_memcpy(void *dst, const void *src, size_t n);
void *ft_mempcpy(void *dst, const void *src, size_t n);

#endif
