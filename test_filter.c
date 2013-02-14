#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "backtrace.h"
#include "filter.h"


int example() {
    char *res = (char *)malloc(1024);
    char const *in = "\n\nSome input.\n";

                          // path,     pname, args...,    NULL
    char * const argv[] = {"/bin/cat", "cat", "-A", "-v", NULL};
    int argc = 5;

    int size = filter(&res, 1024, in, strlen(in), argc, argv);
    if (size == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }
    printf("Wrote %i bytes.\n", size);
    fwrite(res, size, 1, stdout);
    puts("Safe with puts because size != sizeof(res).");
    puts(res);
    free(res);
    return 0;
}

int cppfilt(size_t size) {
    /* Sample backtrace for use with c++filt */
    char *output = (char *)malloc(size);
    // char *output = (char *)malloc(size);
    char * const text = "\
/scratch/tyree/cudafd/src/fd_pricer/py_adi/FiniteDifference/BandedOperatorGPU.so(_Z9backtracePc+0x26) [0x2b097cd2adca]\n\
/scratch/tyree/cudafd/src/fd_pricer/py_adi/FiniteDifference/BandedOperatorGPU.so(_ZN10SizedArrayIdE12sanity_checkEv+0x730) [0x2b097cd2d544]\n\
/scratch/tyree/cudafd/src/fd_pricer/py_adi/FiniteDifference/BandedOperatorGPU.so(_ZN10SizedArrayIdEC1ElSs+0x65) [0x2b097cd3a879]\n\
/scratch/tyree/cudafd/src/fd_pricer/py_adi/FiniteDifference/BandedOperatorGPU.so(_ZN18_CSRBandedOperator5applyER10SizedArrayIdE+0x3fc) [0x2b097cd34570]\n\
/scratch/tyree/cudafd/src/fd_pricer/py_adi/FiniteDifference/BandedOperatorGPU.so(+0x7a550) [0x2b097ccb2550]\n\
/scratch/tyree/cudafd/src/fd_pricer/py_adi/FiniteDifference/BandedOperatorGPU.so(+0x7c367) [0x2b097ccb4367]";
    char * const argv[] = {"/usr/bin/c++filt", "c++filt", NULL};
    char *input = text;
    int argc = 3;
    assert(output);
    assert(input);
    size = demangle(output, size, input);
    printf("Wrote %lu bytes.\n", size);
    fwrite(output, size, 1, stdout);
    puts("\n");
    free(output);
    return 0;
}

int main() {
    cppfilt(255);
    return 0;
}

