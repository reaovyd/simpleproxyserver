/*
 * Common file for sharing macros
 *
 */

#ifndef MACRO_H
#define MACRO_H

#include <stdlib.h>

/* Simple macro to just get rid of Wno-unused-variable */
#define UNUSED(x) ((void)x)

/* Used for the current running server */
#define MAX_BACKLOG_SZ       1024
#define PROXY_SERVER_RUNNING 1

/* Used for command line opt parsing */
#define P_USAGE_EXIT(prog)                              \
    do {                                                \
        fprintf(stderr, "Usage: %s -p <port>\n", prog); \
        exit(EXIT_FAILURE);                             \
    } while(0);                                         \

#endif /* MACRO_H */
