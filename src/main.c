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
    route->max_ttl = atoi(arg);
    break;
  case 'q':
    route->nprobes = atoi(arg);
    break;
  case 'w':
    route->waittime = atoi(arg);
    break;
  case 'p':
    route->udp_port = atoi(arg);
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
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]) {
  int ret;
  t_route route;

  route.max_ttl = DEFAULT_MAX_HOPS;
  route.nprobes = DEFAULT_NPROBES;
  route.waittime = DEFAULT_WAITTIME;
  route.udp_port = DEFAULT_UDP_PORT;

  argp_parse(&argp, argc, argv, 0, 0, &route);

  ret = traceroute(&route);
  return (ret);
}
