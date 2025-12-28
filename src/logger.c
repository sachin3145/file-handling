#include "logger.h"

/*

Currently logs to stderr using fprintf.
Allows logs to be redirected to a file if required without changing call sites.

*/

void logError(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

void perrorCustom(const char *s)
{
    fprintf(stderr, "%s: %s\n", s, strerror(errno));
}

