/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "vm/vm.h"
#include "lexer.h"
#include "compiler.h"

int read_short_option(const char **word_ptr, int words_left) {
    UNUSED(words_left)
    if (strcmp(*word_ptr, "-i") == 0) {
        lexer_state.is_interactive = 1;
        return 1;
    }
    return 0;
}

int read_long_option(const char **word_ptr, int words_left) {
    UNUSED(words_left)
    if (strcmp(*word_ptr, "--interactive") == 0) {
        lexer_state.is_interactive = 1;
        return 1;
    }
    return 0;
}

int main(int argc, const char **argv) {
    setvbuf(stdout, 0, _IOLBF, 0);
    vm_static_init();
    compiler_init();
    const char **curr_arg = argv + 1;
    for (int i = 1; i < argc; i++, curr_arg++) {
        int shift = 0;
        //printf("%s\n",*curr_arg);
        if ((*curr_arg)[0] == '-') {
            // current word are either short or long option
            if ((*curr_arg)[1] == '\0') {
                printf("empty short options are not allowed");
                exit(-2);
            } else if ((*curr_arg)[1] == '-') {
                if ((*curr_arg)[2] == '\0') {
                    printf("empty long options are not allowed");
                    exit(-2);
                }
                shift = read_long_option(curr_arg, argc - i);
            } else {
                shift = read_short_option(curr_arg, argc - i);
            }
        } else {
            FILE *fd;
            if (!(fd = fopen(*curr_arg, "r"))) {
                printf("Unable to open psh file : %s\n", *curr_arg);
                exit(-1);
            }
            lexer_set_in(fd);
            shift = 1;
        }
        if (shift == 0) {
            printf("Option parse error");
            exit(-2);
        }
    }
    struct timeval interval_1;
    struct timeval interval_2;
    gettimeofday(&interval_1, 0);
    lexer_parse();
    gettimeofday(&interval_2, 0);
    const int usec_mult = 1000000;
    long start_time = interval_1.tv_sec * usec_mult + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * usec_mult + interval_2.tv_usec;
    printf("Exit after %ld microseconds\n", (end_time - start_time));

}
