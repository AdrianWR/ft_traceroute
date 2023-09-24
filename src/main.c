#include "traceroute.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

const char *argp_program_version = "ft_traceroute 1.0";

const char *argp_program_bug_address = "<aroque@student.42sp.org.br>";

static char doc[] =
    "traceroute -- Print the route packets trace to network host";

static char args_doc[] = "HOST";

static struct argp_option options[] = {
    {"max-hop", 'm', "MAX_HOPS", 0, "Set maximal hop count (default: 64)", 0},
    {"tries", 'q', "NUM", 0, "Send NUM probe packets per hop (default: 3)", 0},
    {"wait", 'w', "NUM", 0, "Wait NUM seconds for response (default: 3)", 0},
    {"port", 'p', "PORT", 0, "Use destination PORT port (default: 33434)", 0},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct s_route *route = state->input;
  switch (key) {
  case 'm':
    if (validate_max_ttl(arg, &route->max_ttl) == -1)
      return (2);
    break;
  case 'q':
    if (validate_nprobes(arg, &route->nprobes) == -1)
      return (2);
    break;
  case 'w':
    if (validate_waittime(arg, &route->waittime) == -1)
      return (2);
    break;
  case 'p':
    if (validate_udp_port(arg, &route->udp_port) == -1)
      return (2);
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num >= 1)
      argp_usage(state);
    route->host = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1)
      argp_usage(state);
    break;
  default:
    return (ARGP_ERR_UNKNOWN);
  }
  return (0);
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]) {
  int ret;
  t_route route;

  route.max_ttl = DEFAULT_MAX_HOPS;
  route.nprobes = DEFAULT_NPROBES;
  route.waittime = DEFAULT_WAITTIME;
  route.udp_port = DEFAULT_UDP_PORT;

  if ((ret = argp_parse(&argp, argc, argv, 0, 0, &route) != 0))
    return (ret);

  ret = traceroute(&route);
  return (ret);
}
