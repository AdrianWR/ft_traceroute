#include <stdlib.h>

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
  if (!n)
    return (0);
  else if ((*s1 != *s2) || !*s1)
    return ((unsigned char)*s1 - (unsigned char)*s2);
  else
    return (ft_strncmp(++s1, ++s2, --n));
}
