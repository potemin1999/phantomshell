/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/6/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_VM_H
#define SHELL_VM_H

#include "opcodes.h"

typedef uint64_t vm_pc_t;

typedef struct {
    vm_pc_t frame_pc;
    void *stack_ptr;
    void *stack_start;
    void *stack_end;
    void *local_start;
    void *local_end;
} vm_frame_context_t;

typedef vm_pc_t (*vm_opcode_execute_func_t)(vm_frame_context_t *frame, void *data);

int vm_static_init();

int vm_execute_opcode(opcode_t opcode, void *data);

int vm_execute_opcodes(size_t data_len, void *data);

#endif //SHELL_VM_H
