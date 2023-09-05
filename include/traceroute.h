#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

// Main binary parsing options
#define HELP_OPTION 1

typedef struct s_route {
  const char *host;
  unsigned int options;
  struct addrinfo *addrinfo;
  char addr[INET_ADDRSTRLEN];

  int sockfd;
} t_route;

typedef struct s_packet {
  struct iphdr iphdr;
  union {
    struct udphdr udphdr;
    struct icmphdr icmphdr;
  };
  char payload[32];
} t_packet;

extern t_route *g_route;

int traceroute(t_route *route);
int address_lookup(t_route *route);
int init_socket(t_route *route);
t_packet new_packet(uint8_t ttl, uint32_t src, uint32_t dst);
void interrupt_handler(int sig);
void free_route(t_route *route);

size_t ft_strlen(const char *s);
void *ft_memset(void *b, int c, size_t len);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void *ft_memcpy(void *dst, const void *src, size_t n);
void *ft_mempcpy(void *dst, const void *src, size_t n);

#endif
