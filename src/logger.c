#include "logger.h"

/*

Currently logs to stderr using fprintf.
Allows logs to be redirected to a file if required without changing call sites.

*/

int logError(const char *s)
{
    fprintf(stderr, "%s\n", s);
    return EXIT_SUCCESS;
}

int perrorCustom(const char *s)
{
    fprintf(stderr, "%s :%s\n", s, strerror(errno));
    return EXIT_SUCCESS;
}

