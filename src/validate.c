#include "traceroute.h"

static int has_only_digits(const char *s) {
  if (*s == '\0')
    return 1;
  if (ft_isdigit(*s) == 0)
    return 0;
  return has_only_digits(s + 1);
}

int validate_nprobes(const char *arg, unsigned int *nprobes) {
  int ret;
  int n;

  ret = 0;
  if (has_only_digits(arg) == 0) {
    fprintf(stderr, "Cannot handle -q option with arg `%s'\n", arg);
    ret = -1;
  } else if ((n = ft_atoi(arg)) < 1 || n > 10) {
    fprintf(stderr, "no more than 10 probes per hop\n");
    ret = -1;
  } else {
    *nprobes = n;
  }
  return ret;
}

int validate_max_ttl(const char *arg, unsigned short *max_ttl) {
  int ret;
  int n;

  ret = 0;
  if (has_only_digits(arg) == 0) {
    fprintf(stderr, "Cannot handle -m option with arg `%s'\n", arg);
    ret = -1;
  } else if ((n = ft_atoi(arg)) < 1 || n > 255) {
    fprintf(stderr, "max hops cannot be more than 255\n");
    ret = -1;
  } else {
    *max_ttl = n;
  }
  return ret;
}

int validate_waittime(const char *arg, unsigned int *waittime) {
  int ret;

  ret = 0;
  if (has_only_digits(arg) == 0) {
    fprintf(stderr, "Cannot handle -w option with arg `%s'\n", arg);
    ret = -1;
  } else {
    *waittime = ft_atoi(arg);
  }
  return ret;
}

int validate_udp_port(const char *arg, unsigned short *udp_port) {
  int ret;

  ret = 0;
  if (has_only_digits(arg) == 0) {
    fprintf(stderr, "Cannot handle -p option with arg `%s'\n", arg);
    ret = -1;
  } else if ((*udp_port = ft_atoi(arg)) < 1 || *udp_port > 65535) {
    fprintf(stderr, "port must be between 1 and 65535\n");
    ret = -1;
  }
  return ret;
}
