/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/26/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#define GENERATE_STUBS()                        \
    void dump_heap_info() {}                    \
    void dump_stack_info() {}                   \
    void *heap_start_address(){return 0;}       \
    void *heap_current_address(){return 0;}

#ifdef __debug__
#ifdef __simbuild__

#include <malloc.h>
#include <zconf.h>

void *stack_start;

//malloc_stats()
void dump_heap_info() {
    struct mallinfo m = mallinfo();
    const char *template = "heap memory dump:\n"\
    "arena   =%10d  : non-mmapped space allocated from system\n"\
    "ordblks =%10d  : number of free chunks\n"\
    "smblks  =%10d  : number of fastbin blocks\n"\
    "hblks   =%10d  : number of mmapped regions\n"\
    "hblkhd  =%10d  : space in mmapped regions\n"\
    "fsmblks =%10d  : space available in freed fastbin blocks\n"\
    "uordblks=%10d  : total allocated space\n"\
    "fordblks=%10d  : total free space\n"\
    "keepcost=%10d  : top-most, releasable (via malloc_trim) space\n";
    fprintf(stderr, template, m.arena,
           m.ordblks, m.smblks, m.hblks, m.hblkhd,
           m.fsmblks, m.uordblks, m.fordblks, m.keepcost);
}

void dump_stack_info(){

}

void *heap_start_address() {
    if (!stack_start){
        stack_start = sbrk(0);
    }
    return stack_start;
}

void *heap_current_address() {
    return sbrk(0);
}


#else
GENERATE_STUBS()
#endif
#else
GENERATE_STUBS()
#endif