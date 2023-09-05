#include "traceroute.h"
#include <stdint.h>

t_packet new_packet(uint8_t ttl, uint32_t src, uint32_t dst) {
  t_packet packet;

  packet.iphdr.ihl = 5;
  packet.iphdr.version = 4;
  packet.iphdr.tos = 0;
  packet.iphdr.tot_len = sizeof(packet);
  packet.iphdr.id = 0;
  packet.iphdr.frag_off = 0;
  packet.iphdr.ttl = ttl;
  packet.iphdr.protocol = IPPROTO_UDP;
  packet.iphdr.check = 0;
  packet.iphdr.saddr = src;
  packet.iphdr.daddr = dst;

  packet.udphdr.source = htons(0);
  packet.udphdr.dest = htons(33434);
  packet.udphdr.len = htons(sizeof(packet) - sizeof(packet.iphdr));
  packet.udphdr.check = 0;

  return packet;
}
