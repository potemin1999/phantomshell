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
#include "util/gdb_utils.h"
#include "util/hashmap.h"

// amount of memory allocated to constant pool
#define GLOBAL_CONST_POOL_INITIAL_SIZE 4096

// amount of pool constants
#define GLOBAL_CONST_POOL_INITIAL_CAPACITY 64

// initial capacity of global functions array
// and array increment size in case of overflow
#define GLOBAL_FUNCTIONS_COUNT_INCREMENT 64

// capacity of global variables storage in bytes
#define GLOBAL_VARIABLES_CAPACITY 1024

// capacity of global stack in bytes
#define GLOBAL_STACK_CAPACITY 1024

struct {
    void *pool;
    size_t size;
    size_t cap;
    uint32_t *index;
    uint16_t index_size;
    uint16_t index_cap;
} global_const_pool;

struct {
    vm_func_handle_t **funcs;
    size_t funcs_size;
    size_t funcs_cap;
    map_t func_map;
} global_funcs;

typedef vm_pc_t (*vm_opcode_execute_func_t)(vm_frame_context_t *frame, void *data);

vm_opcode_execute_func_t opcode_execute_funcs[OPCODES_COUNT];

vm_frame_context_t call_stack[CALL_STACK_MAX_SIZE];
vm_frame_context_t *root_context = 0;
vm_frame_context_t *current_frame = 0;

static inline void vm_execution_trace(const char *format, ...) {
#ifndef VM_EXECUTION_TRACE
    UNUSED(format)
#else
    printf("\033[34m\033[1mPSVM:\033[21m\033[24m ");
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\033[0m \n");
#endif
}

int vm_execute_opcodes(size_t data_len, void *data) {
    printf("\033[2m\033[36mPSVM: pushed %zu bytes to execution:", data_len);
    struct timeval interval_1;
    struct timeval interval_2;
    gettimeofday(&interval_1, 0);

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

    gettimeofday(&interval_2, 0);
    const int usec_mult = 1000000;
    long start_time = interval_1.tv_sec * usec_mult + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * usec_mult + interval_2.tv_usec;
    printf("\033[2m\033[36mPSVM: %zu bytes executed in %ld microseconds \033[0m\n",
           data_len, (end_time - start_time));
    return (int) pc;
}

static inline
int vm_execute_func_bytecode(size_t data_len, size_t arg_size, size_t ret_size, void *data) {
#ifdef VM_EXECUTION_TRACE
    printf("\033[2m\033[36mPSVM: %zu bytes is to be executed as a function:", data_len);
    ubyte_t *data_bytes = (ubyte_t *) data;
    for (size_t i = 0; i < data_len; i++) {
        printf(" %#x,", data_bytes[i]);
    }
    printf("\033[0m\n");
#endif
    uint16_t *data_uint16 = (uint16_t *) data;
    size_t activation_record_size = sizeof(vm_activation_record_t);
    uint16_t vars_size = be16toh(data_uint16[0]);
    uint16_t stack_size = be16toh(data_uint16[1]);
    uint8_t vars[vars_size];
    uint8_t stack[stack_size + activation_record_size];

    current_frame->stack_ptr -= arg_size;
    memcpy(vars, current_frame->stack_ptr, arg_size);

    // put activation record
    vm_activation_record_t *ar = (vm_activation_record_t *) &stack;
    ar->caller_ar = current_frame->stack_start;
    ar->caller_stack_top = current_frame->stack_ptr;

    // prepare frame data
    current_frame++;
    current_frame->stack_ptr = stack + activation_record_size;
    current_frame->stack_start = stack;
    current_frame->stack_end = stack + stack_size;
    current_frame->local_start = vars;
    current_frame->local_end = vars + vars_size;
    // -3 + 3 will be == to 0, and after tick the bytecode at the beginning of the function will run
    current_frame->frame_pc = 0;

    vm_pc_t pc = 0;
    opcode_t opcode = 0;
    data_len -= 4;
    data += 4;
    while (pc < data_len) {
        opcode = *((ubyte_t *) data + pc);
        vm_pc_t res = opcode_execute_funcs[opcode](current_frame, data + 1 + pc);
        current_frame->frame_pc += res;
        pc += res;
    }

    (current_frame - 1)->stack_ptr += ret_size;
    return (int) ((current_frame--)->frame_pc);
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
    const vm_pc_t pc_shift = 5;
    uint32_t int_data_be = *((uint32_t *) data);
    uint32_t int_data = be32toh(int_data_be);
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    *(int_stack_ptr) = int_data;
    frame->stack_ptr += 4;
    vm_execution_trace("iconst    : %d", int_data);
    return pc_shift;
}

vm_pc_t vm_execute_opcode_ireturn(vm_frame_context_t *frame, void *data) {
    vm_activation_record_t *ar = (vm_activation_record_t *) frame->stack_start;
    int32_t *caller_int_stack = ar->caller_stack_top;
    int32_t *current_int_stack = frame->stack_ptr;
    *caller_int_stack = *(current_int_stack - 1);
    vm_execution_trace("ireturn   : %d to caller stack at %p", *caller_int_stack, caller_int_stack);
    // assign the pc to INT64_MAX to make it greater than the data_len and terminate execution loop
    return (INT64_MAX - frame->frame_pc);
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
    *(float_stack_ptr - 1) = 1.0F * value;
    vm_execution_trace("i2f       : %d -> %f", value, 1.0F * value);
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
    const vm_pc_t pc_shift = 5;
    float_t float_data_be = *((float_t *) data);
    float_t float_data = float_data_be;// be32toh(float_data_be);
    float_t *float_stack_ptr = (float_t *) frame->stack_ptr;
    *(float_stack_ptr) = float_data;
    frame->stack_ptr += 4;
    vm_execution_trace("fconst    : %f", float_data);
    return pc_shift;
}

vm_pc_t vm_execute_opcode_freturn(vm_frame_context_t *frame, void *data) {
    vm_activation_record_t *ar = (vm_activation_record_t *) frame->stack_start;
    float_t *caller_float_stack = ar->caller_stack_top;
    float_t *current_float_stack = frame->stack_ptr;
    *caller_float_stack = *(current_float_stack - 1);
    vm_execution_trace("freturn   : %f to caller stack at %p", *caller_float_stack, caller_float_stack);
    // assign the pc to INT64_MAX to make it greater than the data_len and terminate execution loop
    return (INT64_MAX - frame->frame_pc);
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
    vm_pc_t res = 3;
    return res - jmp_offset;
}

vm_pc_t vm_execute_opcode_call(vm_frame_context_t *frame, void *data) {
    uint16_t be_index = *((uint16_t *) data);
    uint16_t h_index = be16toh(be_index);
    uint32_t pool_offset = global_const_pool.index[h_index];
    const char *constant_raw = (global_const_pool.pool + pool_offset);
    const char *constant = constant_raw + 2;
    vm_func_handle_t *func_handle = 0;
    hashmap_get(global_funcs.func_map, constant, (void **) &func_handle);
    if (!func_handle) {
        vm_do_panic(frame, data, "Method handle not found : %s", constant);
    }

    //TODO: make a workaround without recursion
    vm_execution_trace("call      : %d", h_index);
    vm_execute_func_bytecode(func_handle->bytecode_size, func_handle->arg_size,
                             func_handle->ret_size, func_handle->bytecode_data);
    return 3;
}

vm_pc_t vm_execute_opcode_panic(vm_frame_context_t *frame, void *data) {
    vm_do_panic(frame, data, 0);
}

int vm_register_constant(size_t const_size, const char *constant, vm_pool_const_t *out_index) {
    uint16_t const_size_16 = (uint16_t) const_size;
    // 2 bytes for size, data and null terminator
    uint32_t required_size = 2 + const_size_16 + 1;
    if (global_const_pool.size + required_size > global_const_pool.cap) {
        // allocate new space in the pool to fit new constant
        size_t old_pool_capacity = global_const_pool.cap;
        size_t size_to_allocate = (required_size > GLOBAL_CONST_POOL_INITIAL_SIZE ?
                                   required_size - (old_pool_capacity - global_const_pool.size) :
                                   GLOBAL_CONST_POOL_INITIAL_SIZE);
        size_t new_pool_capacity = old_pool_capacity + size_to_allocate;
        void *new_pool = realloc(global_const_pool.pool, new_pool_capacity);
        vm_execution_trace("expanding root const pool %p[%d] -> %p[%d]",
                           global_const_pool.pool, old_pool_capacity, new_pool, new_pool_capacity);
        global_const_pool.cap = new_pool_capacity;
    }
    if (global_const_pool.index_size == global_const_pool.index_cap) {
        uint16_t old_index_capacity = (uint16_t) global_const_pool.index_cap;
        uint16_t new_index_capacity = (uint16_t) (old_index_capacity + GLOBAL_CONST_POOL_INITIAL_CAPACITY);
        void *new_pool_index = realloc(global_const_pool.index, new_index_capacity);
        vm_execution_trace("expanding root const pool index %p[%d] -> %p[%d]",
                           global_const_pool.index, old_index_capacity, new_pool_index, new_index_capacity);
        global_const_pool.index_cap = new_index_capacity;
    }
    void *write_point = global_const_pool.pool + global_const_pool.size;
    memcpy(write_point, &const_size_16, 2);
    memcpy(write_point + 2, constant, const_size_16);

    *((char *) (write_point + 2 + const_size_16)) = '\0';

    global_const_pool.index[global_const_pool.index_size] = (uint32_t) global_const_pool.size;
    if (out_index) {
        *out_index = global_const_pool.index_size;
    }
    ++global_const_pool.index_size;
    global_const_pool.size += const_size_16 + 2;
    vm_execution_trace("\033[32mREGISTERED\033[34m: root pool const %s at index %u",
                       constant, global_const_pool.index_size - 1);
    return 0;
}

int vm_register_function(vm_pool_const_t signature_index, size_t data_len, void *data, vm_func_handle_t **out_handle) {
    if (global_funcs.funcs_size == global_funcs.funcs_cap) {
        // allocate new space if needed
        size_t old_funcs_capacity = global_funcs.funcs_cap;
        size_t new_funcs_capacity = old_funcs_capacity + GLOBAL_FUNCTIONS_COUNT_INCREMENT;
        vm_func_handle_t *new_funcs = realloc(global_funcs.funcs, sizeof(vm_func_handle_t *) * new_funcs_capacity);
        vm_execution_trace("expanding function pool size %p[%d] -> %p[%d]",
                           global_funcs.funcs, old_funcs_capacity, new_funcs, new_funcs_capacity);
        global_funcs.funcs_cap = new_funcs_capacity;
    }
    // compute args size
    uint32_t signature_offset = global_const_pool.index[signature_index];
    void *signature_data_raw = global_const_pool.pool + signature_offset;
    uint16_t args_size = 0;
    uint16_t ret_size = 0;
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
    ++signature_str_ptr;
    if (*signature_str_ptr == 'I' ||
        *signature_str_ptr == 'F') {
        ret_size += 4;
        ++signature_str_ptr;
        //TODO: process return size of other data types
    } else if (*signature_str_ptr == 'O') {
        // returning object reference
        const size_t objectref_size = 8;
        ret_size += objectref_size;
    }
    // allocate new handle
    vm_func_handle_t *handle = (vm_func_handle_t *) malloc(sizeof(vm_func_handle_t));
    handle->signature_index = signature_index;
    handle->arg_size = args_size;
    handle->ret_size = ret_size;
    uint16_t vars_size_16 = 0;
    uint16_t stack_size_16 = 0;
    memcpy(&vars_size_16, data + 0, 2);
    memcpy(&stack_size_16, data + 2, 2);
    // vars size is written in word counts == 4 bytes
    handle->vars_size = be16toh(vars_size_16) << 2U;
    handle->stack_size = be16toh(stack_size_16);
    handle->bytecode_size = data_len;
    handle->bytecode_data = data;
    // save results
    global_funcs.funcs[global_funcs.funcs_size++] = handle;
    hashmap_put(global_funcs.func_map, signature_str, handle);
    if (out_handle) {
        *out_handle = handle;
    }
    vm_execution_trace("\033[32mREGISTERED\033[34m: root func with signature %s at index %lu;"\
                       "vars=%u, stack=%u, args_size_bytes=%hu",
                       signature_str, global_funcs.funcs_size - 1, handle->vars_size, handle->stack_size,
                       handle->arg_size);
    return 0;
}

int vm_static_init() {
    global_const_pool.pool = malloc(GLOBAL_CONST_POOL_INITIAL_SIZE);
    global_const_pool.size = 0;
    global_const_pool.cap = GLOBAL_CONST_POOL_INITIAL_SIZE;
    global_const_pool.index = malloc(sizeof(void *) * GLOBAL_CONST_POOL_INITIAL_CAPACITY);
    global_const_pool.index_size = 0;
    global_const_pool.index_cap = GLOBAL_CONST_POOL_INITIAL_CAPACITY;

    global_funcs.funcs = malloc(sizeof(vm_func_handle_t) * GLOBAL_FUNCTIONS_COUNT_INCREMENT);
    global_funcs.funcs_cap = GLOBAL_FUNCTIONS_COUNT_INCREMENT;
    global_funcs.funcs_size = 0;
    global_funcs.func_map = hashmap_new();

    call_stack[0].frame_pc = 0;
    call_stack[0].local_start = malloc(GLOBAL_VARIABLES_CAPACITY);
    call_stack[0].local_end = call_stack[0].local_start + GLOBAL_VARIABLES_CAPACITY;
    call_stack[0].stack_start = malloc(GLOBAL_STACK_CAPACITY + sizeof(vm_activation_record_t));
    call_stack[0].stack_ptr = call_stack[0].stack_start + sizeof(vm_activation_record_t);
    call_stack[0].stack_end = call_stack[0].stack_start + GLOBAL_STACK_CAPACITY + sizeof(vm_activation_record_t);

    vm_activation_record_t *ar = (vm_activation_record_t *) call_stack->stack_start;
    ar->caller_ar = 0;
    ar->caller_stack_top = 0;

    current_frame = call_stack + 0;
    root_context = call_stack + 0;

    for (int i = 0; i < OPCODES_COUNT; i++) {
        opcode_execute_funcs[i] = &vm_execute_opcode_panic;
    }
    opcode_execute_funcs[OPCODE_ISAVE] = &vm_execute_opcode_isave;
    opcode_execute_funcs[OPCODE_ILOAD] = &vm_execute_opcode_iload;
    opcode_execute_funcs[OPCODE_ICONST] = &vm_execute_opcode_iconst;
    opcode_execute_funcs[OPCODE_IRETURN] = &vm_execute_opcode_ireturn;
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
    opcode_execute_funcs[OPCODE_FRETURN] = &vm_execute_opcode_freturn;
    opcode_execute_funcs[OPCODE_FADD] = &vm_execute_opcode_fadd;
    opcode_execute_funcs[OPCODE_FSUB] = &vm_execute_opcode_fsub;
    opcode_execute_funcs[OPCODE_FMUL] = &vm_execute_opcode_fmul;
    opcode_execute_funcs[OPCODE_FDIV] = &vm_execute_opcode_fdiv;

    opcode_execute_funcs[OPCODE_JEZ] = &vm_execute_opcode_jez;
    opcode_execute_funcs[OPCODE_JNEZ] = &vm_execute_opcode_jnez;
    opcode_execute_funcs[OPCODE_JMP] = &vm_execute_opcode_jmp;
    opcode_execute_funcs[OPCODE_RJMP] = &vm_execute_opcode_rjmp;

    opcode_execute_funcs[OPCODE_CALL] = &vm_execute_opcode_call;
    vm_load_builtin_functions();
    return 0;
}

int vm_do_panic(vm_frame_context_t *frame, void *data, const char *format, ...) {
    const char *format_text = "\033[91m"\
    "\n\033[1m  *** VM panic ***  \n\033[21m\033[24m"\
    "  Opcode      : %#x, mnemonic=%s\n"\
    "  Frame       : at %p, pc=%p, depth=%d\n"\
    "  Frame stack : %p in [%p;%p]\n"\
    "  AR          : ret to %p\n"\
                              "\033[0m";
    opcode_t opcode = *((uint8_t *) data - 1);
    const char *opcode_name = get_opcode_mnemonic(opcode);
    unsigned long stack_depth = (frame - call_stack) / sizeof(vm_frame_context_t);
    if (stack_depth > CALL_STACK_MAX_SIZE) {
        // standalone
        //stack_depth = 0;
    }
    vm_activation_record_t *record = (vm_activation_record_t *) frame->stack_start;
    fflush(stdout);
    fprintf(stderr, format_text,
            opcode, opcode_name,
            frame, frame->frame_pc, stack_depth,
            frame->stack_ptr, frame->stack_start, frame->stack_end,
            record->caller_stack_top);
    if (format) {
        va_list list;
        va_start(list, format);
        vprintf(format, list);
        va_end(list);
        printf("\n");
    }
    dump_heap_info();
    dump_stack_info();
    exit(EXIT_CODE_VM_PANIC);
}