/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/6/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include <stdlib.h>
#include <stdio.h>
#include <endian.h>
#include <sys/time.h>
#include "vm.h"

//#define VM_EXECUTION_TRACE

vm_opcode_execute_func_t opcode_execute_funcs[256];
vm_frame_context_t *root_context = 0;

//@formatter:off
#ifndef VM_EXECUTION_TRACE
static inline void vm_execution_trace(const char *format, ...) {
    UNUSED(format)
}
#else
#include <stdarg.h>
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
    vm_pc_t pc_add = opcode_execute_funcs[opcode](root_context, data);
    root_context->frame_pc += pc_add;
    return pc_add ? 0 : 1;
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
        vm_pc_t res = opcode_execute_funcs[opcode](root_context, data + 1 + pc);
        root_context->frame_pc += res;
        pc += res;
    }

    gettimeofday(&interval_2, 0);
    long start_time = interval_1.tv_sec * (int) 1e6 + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * (int) 1e6 + interval_2.tv_usec;
    printf("\033[2m\033[36mPSVM: %zu bytes executed in %ld microseconds \033[0m\n",
           data_len, (end_time - start_time));
    return (int) pc;
}


vm_pc_t vm_execute_opcode_isave(vm_frame_context_t *frame, void *data) {
    ubyte_t *byte_data = (ubyte_t *) data;
    ubyte_t index = byte_data[0];
    int32_t *value_ptr = (int32_t *) (frame->stack_ptr - 4);
    *((int32_t *) (frame->local_start + index)) = *value_ptr;
    frame->stack_ptr -= 4;
    vm_execution_trace("isave     : %d to [%u]", *value_ptr, index);
    return 2;
}

vm_pc_t vm_execute_opcode_iload(vm_frame_context_t *frame, void *data) {
    ubyte_t *byte_data = (ubyte_t *) data;
    ubyte_t index = byte_data[0];
    int32_t value = *((int32_t *) (frame->local_start + index));
    int32_t *int_stack_ptr = (int32_t *) frame->stack_ptr;
    *int_stack_ptr = value;
    frame->stack_ptr += 4;
    vm_execution_trace("iload     : %d from [%u]", value, index);
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
    *((float_t *) (frame->local_start + index)) = *value_ptr;
    frame->stack_ptr -= 4;
    vm_execution_trace("fsave     : %f to [%u]", *value_ptr, index);
    return 2;
}

vm_pc_t vm_execute_opcode_fload(vm_frame_context_t *frame, void *data) {
    ubyte_t *byte_data = (ubyte_t *) data;
    ubyte_t index = byte_data[0];
    float_t value = *((float_t *) (frame->local_start + index));
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

vm_pc_t vm_execute_opcode_panic(vm_frame_context_t *frame, void *data) {
    setvbuf(stdout, 0, _IOLBF, 0);
    const char *format_text = "\033[91m"\
    "\033[1m  *** VM panic ***  \n\033[21m\033[24m"\
    "  Opcode      : %#x, mnemonic=%s\n"\
    "  Frame       : at %p, pc=%p\n"\
    "  Frame stack : %p in [%p;%p]"
                              "\033[0m";
    opcode_t opcode = *((uint8_t *) data - 1);
    const char *opcode_name = get_opcode_name(opcode);
    printf(format_text,
           opcode, opcode_name,
           frame, frame->frame_pc,
           frame->stack_ptr, frame->stack_start, frame->stack_end);
    exit(255);
}


int vm_static_init() {
    root_context = malloc(sizeof(vm_frame_context_t));
    root_context->frame_pc = 0;
    root_context->local_start = malloc(1024);
    root_context->local_end = root_context->local_start + 1024;
    root_context->stack_start = malloc(1024);
    root_context->stack_ptr = root_context->stack_start;
    root_context->stack_end = root_context->stack_start + 1024;
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
    return 0;
}