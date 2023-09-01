#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <stdio.h>

// Main binary parsing options
#define HELP_OPTION 1

typedef struct s_route
{
  const char *host;
  unsigned int options;
} t_route;

int ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
