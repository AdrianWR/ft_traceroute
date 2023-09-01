#include "traceroute.h"
#include <stdio.h>

static unsigned int parse_traceroute(int argc, char *argv[], t_route *route)
{
  for (int i = 1; i < argc; i++)
  {
    if (ft_strncmp(argv[i], "--help", 6) == 0)
      route->options |= HELP_OPTION;
    else
      route->host = argv[i];
  }

  return 0;
}

int help()
{
  printf("Usage: traceroute [OPTION...] HOST\n");
  printf("Print the route packets trace to network host.\n");
  printf("\n");
  printf("  --help     display this help and exit\n");
  return 0;
}

int main(int argc, char *argv[])
{
  t_route route;

  if (parse_traceroute(argc, argv, &route) != 0)
    return 1;

  if (route.options & HELP_OPTION || argc == 1)
    return help();

  printf("Host: %s\n", route.host);
  return 0;
}
