/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/26/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_GDB_UTILS_H
#define PHANTOMSHELL_GDB_UTILS_H

void dump_heap_info();

void dump_stack_info();

void *heap_start_address();

void *heap_current_address();

#endif //PHANTOMSHELL_GDB_UTILS_H
