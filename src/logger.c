#include "logger.h"

/*

Currently logs to stderr using fprintf.
Allows logs to be redirected to a file if required without changing call sites.

*/

void logError(const char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}

void logWarn(const char *s)
{
    fprintf(stderr, "Warning: %s\n", s);
}

void logSysError(const char *s)
{
    fprintf(stderr, "SysError: %s (errno %d)\nReason: %s\n", s, errno,strerror(errno));
}

