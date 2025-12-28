#ifndef LOGGER_H
#define LOGGER_H

/* INCLUDES */
#include<stdio.h>
#include<string.h>
#include<errno.h>

/* MACRO FUNCTIONS */
#define CHECK_NULL(ptr, retval)              \
    do {                             \
        if ((ptr) == NULL) {         \
            logError(#ptr " is NULL"); \
            return (retval);               \
        }                            \
    } while (0)

#define STR(x) #x
#define XSTR(x) STR(x)


/* FORWARD DECLARATIONS */
void logError(const char *s);
void logWarn(const char *s);
void logSysError(const char *s);


#endif // LOGGER_H
