#include <string.h>
#include "xnu-definitions.h"

size_t strlcpy(char *dst, const char *src, size_t n)
{
    char *p = dst;

    if (n != 0)
    {
        for (; --n != 0; p++, src++)
        {
            if ((*p = *src) == '\0')
                return p - dst;
        }
        *p = '\0';
    }
    return (p - dst) + strlen(src);
}

size_t strlcat(char *dst, const char *src, size_t n)
{
    char *p = dst;

    while (n != 0 && *p != '\0')
    {
        p++;
        n--;
    }
    if (n != 0)
    {
        for (; --n != 0; p++, src++)
        {
            if ((*p = *src) == '\0')
                return p - dst;
        }
        *p = '\0';
    }
    return (p - dst) + strlen(src);
}