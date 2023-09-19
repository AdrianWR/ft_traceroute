#include <sys/time.h>

float time_diff_ms(struct timeval *start, struct timeval *end) {
  return (end->tv_sec - start->tv_sec) * 1000.0 +
         (end->tv_usec - start->tv_usec) / 1000.0;
}
