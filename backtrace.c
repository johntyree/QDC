#ifndef BACKTRACE_H
#define BACKTRACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "filter.h"


inline char* demangle2(char *bt) {
    char *output = NULL;
    char * const argv[] = {"/usr/bin/c++filt", "c++filt", NULL};
    int argc = 3;
    filter(&output, 0, bt, strlen(bt), argc, argv);
    return output;
}


inline char* demangle(char *dest, size_t n, char *bt) {
    char *output = demangle2(bt);
    size_t s = strlen(output);
    n = s > n ? n : s;
    memcpy(dest, output, n);
    free(output);
    return dest;
}

inline char* get_backtrace(char *full, size_t size) {
    int j, nptrs;
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, size);
    // printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
        would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols() failed.");
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < nptrs; j++) {
        strcat(full, strings[j]);
        strcat(full, "\n");
    }

    free(strings);
    return full;
}

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* end of include guard */
