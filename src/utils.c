#include <stdlib.h>

size_t ft_strlen(const char *s) { return (!*s ? 0 : ft_strlen(s + 1) + 1); }

void *ft_memset(void *b, int c, size_t len) {
  unsigned char *p;

  p = b;
  while (len--)
    *p++ = (unsigned char)c;
  return (b);
}

int ft_strncmp(const char *s1, const char *s2, size_t n) {
  if (!n)
    return (0);
  else if ((*s1 != *s2) || !*s1)
    return ((unsigned char)*s1 - (unsigned char)*s2);
  else
    return (ft_strncmp(++s1, ++s2, --n));
}

void *ft_memcpy(void *dst, const void *src, size_t n) {
  unsigned char *s;
  unsigned char *d;

  s = (unsigned char *)src;
  d = (unsigned char *)dst;
  while (n-- && dst != src)
    *d++ = *s++;
  return (dst);
}

void *ft_mempcpy(void *dst, const void *src, size_t n) {
  return (ft_memcpy(dst, src, n) + n);
}

int ft_isdigit(int c) { return (c >= '0' && c <= '9'); }

int ft_isspace(int str) {
  if (str == 0x20 || (str >= 0x09 && str <= 0x0d))
    return (1);
  return (0);
}

int ft_atoi(const char *str) {
  int n;
  int aux;
  int signal;

  n = 0;
  aux = 0;
  signal = -1;
  while (ft_isspace(*str))
    str++;
  if (*str == '+' || *str == '-')
    if (*str++ == '-')
      signal = 1;
  while (*str >= '0' && *str <= '9') {
    n = n * 10 - (*str++ - '0');
    if (aux < n) {
      if (signal < 0)
        return (-1);
      return (0);
    }
    aux = n;
  }
  return (n * signal);
}
