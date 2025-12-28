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


/* FORWARD DECLARATIONS */
void logError(const char *s);
void perrorCustom(const char *s);


#endif // LOGGER_H
