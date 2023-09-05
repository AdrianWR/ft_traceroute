#include <stdlib.h>

size_t ft_strlen(const char *s)
{
  return (!*s ? 0 : ft_strlen(s + 1) + 1);
}

void *ft_memset(void *b, int c, size_t len)
{
  unsigned char *p;

  p = b;
  while (len--)
    *p++ = (unsigned char)c;
  return (b);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
  if (!n)
    return (0);
  else if ((*s1 != *s2) || !*s1)
    return ((unsigned char)*s1 - (unsigned char)*s2);
  else
    return (ft_strncmp(++s1, ++s2, --n));
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
  unsigned char *s;
  unsigned char *d;

  s = (unsigned char *)src;
  d = (unsigned char *)dst;
  while (n-- && dst != src)
    *d++ = *s++;
  return (dst);
}

void *ft_mempcpy(void *dst, const void *src, size_t n)
{
  return (ft_memcpy(dst, src, n) + n);
}
