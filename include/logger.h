#ifndef LOGGER_H
#define LOGGER_H

/* INCLUDES */
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

/* MACRO FUNCTIONS */
#define CHECK_NULL(ptr)              \
    do {                             \
        if ((ptr) == NULL) {         \
            logError(#ptr " is NULL"); \
            return EXIT_FAILURE;               \
        }                            \
    } while (0)


/* FORWARD DECLARATIONS */
int logError(const char *s);
int perrorCustom(const char *s);


#endif // LOGGER_H
