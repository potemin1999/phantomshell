/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include <stdio.h>
#include <sys/time.h>
#include "vm/vm.h"
#include "lexer.h"

int main(int argc, const char **argv) {
    setvbuf(stdout, 0, _IOLBF, 0);
    vm_static_init();
    if (argc > 1) {
        FILE *fd;
        if (!(fd = fopen(argv[1], "r"))) {
            printf("%s\n", argv[1]);
            perror("Error : ");
            return (-1);
        }
        lexer_set_in(fd);
    }
    struct timeval interval_1;
    struct timeval interval_2;
    gettimeofday(&interval_1, 0);
    lexer_parse();
    gettimeofday(&interval_2, 0);
    long start_time = interval_1.tv_sec * (int) 1e6 + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * (int) 1e6 + interval_2.tv_usec;
    printf("Exit after %ld microseconds\n", (end_time - start_time));

}
