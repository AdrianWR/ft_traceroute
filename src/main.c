#include "traceroute.h"
#include <stdio.h>
#include <stdlib.h>

t_route *g_route;

static unsigned int parse_traceroute(int argc, char *argv[], t_route *route) {
  for (int i = 1; i < argc; i++) {
    if (ft_strncmp(argv[i], "--help", 6) == 0)
      route->options |= HELP_OPTION;
    else
      route->host = argv[i];
  }

  return 0;
}

int help() {
  printf("Usage: traceroute [OPTION...] HOST\n");
  printf("Print the route packets trace to network host.\n");
  printf("\n");
  printf("  --help     display this help and exit\n");
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;

  if (!(g_route = malloc(sizeof(*g_route)))) {
    fprintf(stderr, "traceroute: malloc failed\n");
    return 1;
  }
  ft_memset(g_route, 0, sizeof(*g_route));

  if (parse_traceroute(argc, argv, g_route) != 0) {
    fprintf(stderr, "traceroute: parse failed\n");
    return 1;
  }

  if (g_route->options & HELP_OPTION || argc == 1)
    return help();

  ret = traceroute(g_route);
  free(g_route);
  return (ret);
}
