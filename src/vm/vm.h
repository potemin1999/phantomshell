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

#define EXIT_CODE_VM_PANIC 255
#define CALL_STACK_MAX_SIZE 128

typedef uint16_t vm_pool_const_t;
typedef int64_t vm_pc_t;

typedef struct {
    // current program counter
    vm_pc_t frame_pc;
    // pointer to the current stack top (free place)
    void *stack_ptr;
    // pointer to the stack beginning
    void *stack_start;
    // pointer to the stack end
    void *stack_end;
    // pointer to the local variables start
    void *local_start;
    // pointer to the local variables end
    void *local_end;
} vm_frame_context_t;

typedef struct {
    // constant reference to the method signature name
    uint16_t signature_index;
    // size of arguments which should be copied from the stack top
    uint16_t arg_size;
    // count of local variables
    uint32_t vars_size;
    // stack size to allocate
    uint32_t stack_size;
    // function bytecode size
    size_t bytecode_size;
    // function bytecode
    void *bytecode_data;
} vm_func_handle_t;

int vm_static_init();

__attribute((noreturn)) __attribute((noinline))
int vm_do_panic(vm_frame_context_t *frame, void *data, const char *format, ...);

__attribute((noinline))
int vm_execute_opcodes(size_t data_len, void *data);

int vm_register_constant(size_t const_size, const char *constant, vm_pool_const_t *out_index);

int vm_register_function(vm_pool_const_t signature_index, size_t data_len, void *data, vm_func_handle_t **out_handle);

int vm_register_class(vm_pool_const_t name_index, size_t data_len, void *data);

#endif //SHELL_VM_H
