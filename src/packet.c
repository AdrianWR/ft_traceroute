#include "traceroute.h"
#include <stdint.h>

t_packet new_packet() {
  t_packet packet;

  // Fill up packet payload with incrementing bytes, starting from 0x40
  for (int i = 0; i < 32; i++) {
    packet.payload[i] = 0x40 + i;
  }

  return packet;
}
