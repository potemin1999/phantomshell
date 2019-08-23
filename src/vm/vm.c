/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/6/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include "lib.h"
#include "vm/vm.h"
#include "util/hashmap.h"

typedef struct {
    void *pool;
    size_t pool_size;
    size_t pool_cap;
    uint32_t *pool_index;
    uint16_t pool_index_size;
    uint16_t pool_index_cap;
} vm_root_const_pool_t;

vm_root_const_pool_t root_const_pool;

typedef struct {
    vm_func_handle_t **funcs;
    size_t funcs_size;
    size_t funcs_cap;
    map_t func_map;
} vm_root_funcs_t;

vm_root_funcs_t root_funcs;

vm_opcode_execute_func_t opcode_execute_funcs[256];
vm_frame_context_t *root_context = 0;

vm_frame_context_t call_stack[128];
vm_frame_context_t *current_frame = 0;

//@formatter:off
#ifndef VM_EXECUTION_TRACE
static inline void vm_execution_trace(const char *format, ...) {
    UNUSED(format)
}
#else
static inline void vm_execution_trace(const char *format, ...) {
    printf("\033[34m\033[1mPSVM:\033[21m\033[24m ");
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\033[0m \n");
}
#endif
//@formatter:on


int vm_execute_opcode(opcode_t opcode, void *data) {
    //printf("pushed single bytecode to execution\n");
    vm_pc_t pc_add = opcode_execute_funcs[opcode](current_frame, data);
    current_frame->frame_pc += pc_add;
    return pc_add ? 0 : 1;
}

int vm_execute_opcodes(size_t data_len, void *data) {
    printf("\033[2m\033[36mPSVM: pushed %zu bytes to execution:", data_len);
    //struct timeval interval_1;
    //struct timeval interval_2;
    //gettimeofday(&interval_1, 0);

    ubyte_t *data_bytes = (ubyte_t *) data;
    for (size_t i = 0; i < data_len; i++) {
        printf(" %#x,", data_bytes[i]);
    }
    printf("\033[0m\n");
    vm_pc_t pc = 0;
    opcode_t opcode = 0;
    while (pc < data_len) {
        opcode = *((ubyte_t *) data + pc);
        vm_pc_t res = opcode_execute_funcs[opcode](current_frame, data + 1 + pc);
        current_frame->frame_pc += res;
        pc += res;
    }

    //gettimeofday(&interval_2, 0);
    //long start_time = interval_1.tv_sec * (int) 1e6 + interval_1.tv_usec;
    //long end_time = interval_2.tv_sec * (int) 1e6 + interval_2.tv_usec;
    //printf("\033[2m\033[36mPSVM: %zu bytes executed in %ld microseconds \033[0m\n",
    //       data_len, (end_time - start_time));
    return (int) pc;
}


vm_pc_t vm_execute_opcode_isave(vm_frame_context_t *frame, void *data) {
    ubyte_t index = ((ubyte_t *) data)[0];
    int32_t *value_ptr = (int32_t *) (frame->stack_ptr - 4);
    *((int32_t *) (frame->local_start + index * 4)) = *value_ptr;
    frame->stack_ptr -= 4;
    vm_execution_trace("isave     : %d to [%u]", *value_ptr, index);
    return 2;
}

vm_pc_t vm_execute_opcode_iload(vm_frame_context_t *frame, void *data) {
    ubyte_t index = ((ubyte_t *) data)[0];
    int32_t value = *((int32_t *) (frame->local_start + index * 4));
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    *int_stack_ptr = value;
    frame->stack_ptr += 4;
    vm_execution_trace("iload     : %d from [%u]", *int_stack_ptr, index);
    return 2;
}

vm_pc_t vm_execute_opcode_iconst(vm_frame_context_t *frame, void *data) {
    uint32_t int_data_be = *((uint32_t *) data);
    uint32_t int_data = be32toh(int_data_be);
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    *(int_stack_ptr) = int_data;
    frame->stack_ptr += 4;
    vm_execution_trace("iconst    : %d", int_data);
    return 5;
}

vm_pc_t vm_execute_opcode_iadd(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = value_1 + value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("iadd      : %d + %d", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_isub(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = value_1 - value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("isub      : %d - %d", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_imul(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = value_1 * value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("imul      : %d * %d", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_idiv(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = value_1 / value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("idiv      : %d / %d", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_ieq(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = (value_1 == value_2 ? 1 : 0);
    frame->stack_ptr -= 4;
    vm_execution_trace("ieq       : %d == %d", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_ineq(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = (value_1 != value_2 ? 1 : 0);
    frame->stack_ptr -= 4;
    vm_execution_trace("ineq      : %d != %d", value_1, value_2);
    return 1;
}


vm_pc_t vm_execute_opcode_ilt(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = (value_1 < value_2 ? 1 : 0);
    vm_execution_trace("ilt       : %d < %d", value_1, value_2);
    frame->stack_ptr -= 4;
    return 1;
}

vm_pc_t vm_execute_opcode_ingt(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = (value_1 <= value_2 ? 1 : 0);
    vm_execution_trace("ingt      : %d <= %d", value_1, value_2);
    frame->stack_ptr -= 4;
    return 1;
}

vm_pc_t vm_execute_opcode_igt(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = (value_1 > value_2 ? 1 : 0);
    vm_execution_trace("igt       : %d > %d", value_1, value_2);
    frame->stack_ptr -= 4;
    return 1;
}

vm_pc_t vm_execute_opcode_inlt(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_2 = *(int_stack_ptr - 1);
    int32_t value_1 = *(int_stack_ptr - 2);
    *(int_stack_ptr - 2) = (value_1 >= value_2 ? 1 : 0);
    vm_execution_trace("inlt      : %d >= %d", value_1, value_2);
    frame->stack_ptr -= 4;
    return 1;
}

vm_pc_t vm_execute_opcode_i2f(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    int32_t value = *(int_stack_ptr - 1);
    *(float_stack_ptr - 1) = 1.0f * value;
    vm_execution_trace("i2f       : %d -> %f", value, 1.0f * value);
    return 1;
}


vm_pc_t vm_execute_opcode_fsave(vm_frame_context_t *frame, void *data) {
    ubyte_t *byte_data = (ubyte_t *) data;
    ubyte_t index = byte_data[0];
    float_t *value_ptr = (float_t *) (frame->stack_ptr - 4);
    *((float_t *) (frame->local_start + index * 4)) = *value_ptr;
    frame->stack_ptr -= 4;
    vm_execution_trace("fsave     : %f to [%u]", *value_ptr, index);
    return 2;
}

vm_pc_t vm_execute_opcode_fload(vm_frame_context_t *frame, void *data) {
    ubyte_t *byte_data = (ubyte_t *) data;
    ubyte_t index = byte_data[0];
    float_t value = *((float_t *) (frame->local_start + index * 4));
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    *float_stack_ptr = value;
    frame->stack_ptr += 4;
    vm_execution_trace("fload     : %f from [%u]", value, index);
    return 2;
}

vm_pc_t vm_execute_opcode_fconst(vm_frame_context_t *frame, void *data) {
    float_t float_data_be = *((float_t *) data);
    float_t float_data = float_data_be;// be32toh(float_data_be);
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    *(float_stack_ptr) = float_data;
    frame->stack_ptr += 4;
    vm_execution_trace("fconst    : %f", float_data);
    return 5;
}

vm_pc_t vm_execute_opcode_fadd(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    float_t value_2 = *(float_stack_ptr - 1);
    float_t value_1 = *(float_stack_ptr - 2);
    *(float_stack_ptr - 2) = value_1 + value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("fadd      : %f + %f", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_fsub(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    float_t value_2 = *(float_stack_ptr - 1);
    float_t value_1 = *(float_stack_ptr - 2);
    *(float_stack_ptr - 2) = value_1 - value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("fsub      : %f - %f", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_fmul(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    float_t value_2 = *(float_stack_ptr - 1);
    float_t value_1 = *(float_stack_ptr - 2);
    *(float_stack_ptr - 2) = value_1 * value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("fmul      : %f * %f", value_1, value_2);
    return 1;
}

vm_pc_t vm_execute_opcode_fdiv(vm_frame_context_t *frame, void *data) {
    UNUSED(data)
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    float_t value_2 = *(float_stack_ptr - 1);
    float_t value_1 = *(float_stack_ptr - 2);
    *(float_stack_ptr - 2) = value_1 / value_2;
    frame->stack_ptr -= 4;
    vm_execution_trace("fdiv      : %f / %f", value_1, value_2);
    return 1;
}


vm_pc_t vm_execute_opcode_jez(vm_frame_context_t *frame, void *data) {
    uint16_t jmp_offset = *((uint16_t *) data);
    jmp_offset = be16toh(jmp_offset);
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_1 = *(int_stack_ptr - 1);
    frame->stack_ptr -= 4;
    vm_pc_t pc_offset = (value_1 == 0 ? (vm_pc_t) jmp_offset : 0);
    vm_execution_trace("jez       : %d", jmp_offset);
    return 3 + pc_offset;
}

vm_pc_t vm_execute_opcode_jnez(vm_frame_context_t *frame, void *data) {
    uint16_t jmp_offset = *((uint16_t *) data);
    jmp_offset = be16toh(jmp_offset);
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    int32_t value_1 = *(int_stack_ptr - 1);
    frame->stack_ptr -= 4;
    vm_pc_t pc_offset = (value_1 == 0 ? 0 : (vm_pc_t) jmp_offset);
    vm_execution_trace("jnez      : %d", jmp_offset);
    return 3 + pc_offset;
}

vm_pc_t vm_execute_opcode_jmp(vm_frame_context_t *frame, void *data) {
    UNUSED(frame)
    uint16_t jmp_offset = *((uint16_t *) data);
    jmp_offset = be16toh(jmp_offset);
    vm_execution_trace("jmp       : %d", jmp_offset);
    return 3 + jmp_offset;
}

vm_pc_t vm_execute_opcode_rjmp(vm_frame_context_t *frame, void *data) {
    UNUSED(frame)
    uint16_t jmp_offset = *((uint16_t *) data);
    jmp_offset = be16toh(jmp_offset);
    vm_execution_trace("jmp       : %d", jmp_offset);
    return 3 - jmp_offset;
}

vm_pc_t vm_execute_opcode_call(vm_frame_context_t *frame, void *data) {
    uint16_t be_index = *((uint16_t *) data);
    uint16_t h_index = be16toh(be_index);
    uint32_t pool_offset = root_const_pool.pool_index[h_index];
    const char *constant_raw = (root_const_pool.pool + pool_offset);
    const char *constant = constant_raw + 2;
    vm_func_handle_t *func_handle = 0;
    hashmap_get(root_funcs.func_map, constant, (void **) &func_handle);
    if (!func_handle) {
        vm_do_panic(frame, data, "Method handle not found : %s", constant);
    }

    //TODO: fix stack size counter
    uint8_t *stack = (uint8_t *) malloc(64);
    uint8_t *vars = (uint8_t *) malloc(func_handle->vars_size);
    uint16_t arg_size = func_handle->arg_size;

    memcpy(vars, current_frame->stack_ptr - arg_size, arg_size);

    current_frame++;
    current_frame->stack_ptr = stack;
    current_frame->stack_start = stack;
    current_frame->stack_end = stack + 64;
    current_frame->local_start = vars;
    current_frame->local_end = vars + func_handle->vars_size;
    // -3 + 3 will be == to 0, and after tick the bytecode at the beginning of the function will run
    current_frame->frame_pc = 0;
    //TODO: make a workaround without recursion
    vm_execution_trace("call      : %d", h_index);
    vm_execute_opcodes(func_handle->bytecode_size - 4, func_handle->bytecode_data + 4);
    free(current_frame->stack_start);
    free(current_frame->local_start);
    current_frame--;
    return 3;
}

vm_pc_t vm_execute_opcode_panic(vm_frame_context_t *frame, void *data) {
    vm_do_panic(frame, data, 0);
}

int vm_register_constant(size_t const_size, const char *constant, vm_pool_const_t *out_index) {
    uint16_t const_size_16 = const_size;
    // 2 bytes for size, data and null terminator
    uint32_t required_size = 2 + const_size_16 + 1;
    if (root_const_pool.pool_size + required_size > root_const_pool.pool_cap) {
        // allocate new space in the pool to fit new constant
        size_t old_pool_capacity = root_const_pool.pool_cap;
        size_t size_to_allocate = (required_size > 4096 ?
                                   required_size - (old_pool_capacity - root_const_pool.pool_size) : 4096);
        size_t new_pool_capacity = old_pool_capacity + size_to_allocate;
        void *new_pool = realloc(root_const_pool.pool, new_pool_capacity);
        vm_execution_trace("expanding root const pool %p[%d] -> %p[%d]",
                           root_const_pool.pool, old_pool_capacity, new_pool, new_pool_capacity);
        root_const_pool.pool_cap = new_pool_capacity;
    }
    if (root_const_pool.pool_index_size == root_const_pool.pool_index_cap) {
        uint16_t old_index_capacity = root_const_pool.pool_index_cap;
        uint16_t new_index_capacity = old_index_capacity + 64;
        void *new_pool_index = realloc(root_const_pool.pool_index, new_index_capacity);
        vm_execution_trace("expanding root const pool index %p[%d] -> %p[%d]",
                           root_const_pool.pool_index, old_index_capacity, new_pool_index, new_index_capacity);
        root_const_pool.pool_index_cap = new_index_capacity;
    }
    void *write_point = root_const_pool.pool + root_const_pool.pool_size;
    memcpy(write_point, &const_size_16, 2);
    memcpy(write_point + 2, constant, const_size_16);

    *((char *) (write_point + 2 + const_size_16)) = '\0';

    root_const_pool.pool_index[root_const_pool.pool_index_size] = root_const_pool.pool_size;
    if (out_index) {
        *out_index = root_const_pool.pool_index_size;
    }
    ++root_const_pool.pool_index_size;
    vm_execution_trace("\033[32mREGISTERED\033[34m: root pool const %s at index %u",
                       constant, root_const_pool.pool_index_size - 1);
    return 0;
}

int vm_register_function(vm_pool_const_t signature_index, size_t data_len, void *data, vm_func_handle_t **out_handle) {
    if (root_funcs.funcs_size == root_funcs.funcs_cap) {
        // allocate new space if needed
        size_t old_funcs_capacity = root_funcs.funcs_cap;
        size_t new_funcs_capacity = old_funcs_capacity + 64;
        vm_func_handle_t *new_funcs = realloc(root_funcs.funcs, sizeof(vm_func_handle_t *) * new_funcs_capacity);
        vm_execution_trace("expanding function pool size %p[%d] -> %p[%d]",
                           root_funcs.funcs, old_funcs_capacity, new_funcs, new_funcs_capacity);
        root_funcs.funcs_cap = new_funcs_capacity;
    }
    // compute args size
    uint32_t signature_offset = root_const_pool.pool_index[signature_index];
    void *signature_data_raw = root_const_pool.pool + signature_offset;
    uint16_t args_size = 0;
    const char *signature_str = signature_data_raw + 2;
    const char *signature_str_ptr = signature_str;
    while (*signature_str_ptr != '(') { signature_str_ptr++; }
    ++signature_str_ptr;
    while (*signature_str_ptr != ')') {
        if (*signature_str_ptr == 'I' ||
            *signature_str_ptr == 'F') {
            args_size += 4;
            ++signature_str_ptr;
            continue;
        }
        //TODO: process argument size of other data types
        vm_do_panic(root_context, data, "Unknown function signature element : in %s at position %zu",
                    signature_str, (size_t) (signature_str_ptr - signature_str));
    }
    // allocate new handle
    //TODO: change back to the malloc without getting errors from the valgrind
    vm_func_handle_t *handle = (vm_func_handle_t *) calloc(1, sizeof(vm_func_handle_t));
    handle->signature_index = signature_index;
    handle->arg_size = args_size;
    //TODO: add big endian support
    memcpy(&(handle->vars_size), data + 0, 2);
    memcpy(&(handle->stack_size), data + 2, 2);
    handle->vars_size = be32toh(handle->vars_size);
    handle->stack_size = be32toh(handle->stack_size);
    handle->bytecode_size = data_len;
    handle->bytecode_data = data;
    // save results
    root_funcs.funcs[root_funcs.funcs_size++] = handle;
    hashmap_put(root_funcs.func_map, signature_str, handle);
    if (out_handle) {
        *out_handle = handle;
    }
    vm_execution_trace("\033[32mREGISTERED\033[34m: root func with signature %s at index %lu;"\
                       "vars=%u, stack=%u, args_size_bytes=%hu",
                       signature_str, root_funcs.funcs_size - 1, handle->vars_size, handle->stack_size,
                       handle->arg_size);
    return 0;
}

int vm_static_init() {
    root_const_pool.pool = malloc(4096);
    root_const_pool.pool_size = 0;
    root_const_pool.pool_cap = 4096;
    root_const_pool.pool_index = malloc(sizeof(void *) * 64);
    root_const_pool.pool_index_size = 0;
    root_const_pool.pool_index_cap = 64;

    root_funcs.funcs = malloc(sizeof(vm_func_handle_t) * 64);
    root_funcs.funcs_cap = 64;
    root_funcs.funcs_size = 0;
    root_funcs.func_map = hashmap_new();

    //call_stack[0] = malloc(sizeof(vm_frame_context_t));
    call_stack[0].frame_pc = 0;
    call_stack[0].local_start = malloc(1024);
    call_stack[0].local_end = call_stack[0].local_start + 1024;
    call_stack[0].stack_start = malloc(1024);
    call_stack[0].stack_ptr = call_stack[0].stack_start;
    call_stack[0].stack_end = call_stack[0].stack_start + 1024;
    current_frame = call_stack + 0;

    for (int i = 0; i < 256; i++) {
        opcode_execute_funcs[i] = &vm_execute_opcode_panic;
    }
    opcode_execute_funcs[OPCODE_ISAVE] = &vm_execute_opcode_isave;
    opcode_execute_funcs[OPCODE_ILOAD] = &vm_execute_opcode_iload;
    opcode_execute_funcs[OPCODE_ICONST] = &vm_execute_opcode_iconst;
    opcode_execute_funcs[OPCODE_IADD] = &vm_execute_opcode_iadd;
    opcode_execute_funcs[OPCODE_ISUB] = &vm_execute_opcode_isub;
    opcode_execute_funcs[OPCODE_IMUL] = &vm_execute_opcode_imul;
    opcode_execute_funcs[OPCODE_IDIV] = &vm_execute_opcode_idiv;
    opcode_execute_funcs[OPCODE_IEQ] = &vm_execute_opcode_ieq;
    opcode_execute_funcs[OPCODE_INEQ] = &vm_execute_opcode_ineq;
    opcode_execute_funcs[OPCODE_ILT] = &vm_execute_opcode_ilt;
    opcode_execute_funcs[OPCODE_IGT] = &vm_execute_opcode_igt;
    opcode_execute_funcs[OPCODE_INLT] = &vm_execute_opcode_inlt;
    opcode_execute_funcs[OPCODE_INGT] = &vm_execute_opcode_ingt;
    opcode_execute_funcs[OPCODE_I2F] = &vm_execute_opcode_i2f;

    opcode_execute_funcs[OPCODE_FSAVE] = &vm_execute_opcode_fsave;
    opcode_execute_funcs[OPCODE_FLOAD] = &vm_execute_opcode_fload;
    opcode_execute_funcs[OPCODE_FCONST] = &vm_execute_opcode_fconst;
    opcode_execute_funcs[OPCODE_FADD] = &vm_execute_opcode_fadd;
    opcode_execute_funcs[OPCODE_FSUB] = &vm_execute_opcode_fsub;
    opcode_execute_funcs[OPCODE_FMUL] = &vm_execute_opcode_fmul;
    opcode_execute_funcs[OPCODE_FDIV] = &vm_execute_opcode_fdiv;

    opcode_execute_funcs[OPCODE_JEZ] = &vm_execute_opcode_jez;
    opcode_execute_funcs[OPCODE_JNEZ] = &vm_execute_opcode_jnez;
    opcode_execute_funcs[OPCODE_JMP] = &vm_execute_opcode_jmp;
    opcode_execute_funcs[OPCODE_RJMP] = &vm_execute_opcode_rjmp;

    opcode_execute_funcs[OPCODE_CALL] = &vm_execute_opcode_call;
    return 0;
}

int vm_do_panic(vm_frame_context_t *frame, void *data, const char *format, ...) {
    setvbuf(stdout, 0, _IOLBF, 0);
    const char *format_text = "\033[91m"\
    "\033[1m  *** VM panic ***  \n\033[21m\033[24m"\
    "  Opcode      : %#x, mnemonic=%s\n"\
    "  Frame       : at %p, pc=%p\n"\
    "  Frame stack : %p in [%p;%p]\n"
                              "\033[0m";
    opcode_t opcode = *((uint8_t *) data - 1);
    const char *opcode_name = get_opcode_mnemonic(opcode);
    printf(format_text,
           opcode, opcode_name,
           frame, frame->frame_pc,
           frame->stack_ptr, frame->stack_start, frame->stack_end);
    if (format) {
        va_list list;
        va_start(list, format);
        vprintf(format, list);
        va_end(list);
        printf("\n");
    }
    exit(255);
}