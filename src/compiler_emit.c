/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/6/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdlib.h>
//#include <stdarg.h>
#include "compiler.h"
#include "vm/vm.h"

size_t emitter_emit_raw(struct bytecode_emitter_t *emitter, size_t n, void *data);

size_t emitter_emit_unbuffered(struct bytecode_emitter_t *emitter, /*opcode_t opcode,*/ size_t n, void *data) {
    UNUSED(emitter)
    UNUSED(n)
    //printf("emit [%d] %s\n", n, get_opcode_mnemonic(opcode));

    //vm_execute_opcode(opcode, data);
    vm_execute_opcodes(n, data);
    return 0;
}

struct bytecode_emitter_t compiler_emitter_unbuffered_new() {
    struct bytecode_emitter_t emitter;
    //emitter.raw_emitter_func = emitter_emit_raw;
    emitter.emitter_func = emitter_emit_unbuffered;
    emitter.capacity = INT32_MAX;
    emitter.page_size = INT32_MAX;
    emitter.size = 0;
    emitter.first_page = 0;
    emitter.last_page = 0;
    return emitter;
}

void *emitter_allocate_new_page(size_t page_size, void *prev_page) {
    // keep 8 bytes for the link to the next page
    size_t real_page_size = page_size + sizeof(void *);
    void *new_page = malloc(real_page_size);
    bzero(new_page, real_page_size);
    if (prev_page) {
        void **next_ptr = (void **) (prev_page + page_size);
        *next_ptr = new_page;
    }
    return new_page;
}

size_t emitter_emit_buffered(struct bytecode_emitter_t *emitter, /*opcode_t opcode,*/ size_t n, void *data) {
    // check if there is enough space to write more
    void *emitter_page;
    size_t page_size = emitter->page_size;
    if (emitter->size + n > emitter->capacity) {
        // we need to allocate new page
        void *prev_emitter_page = emitter->last_page;
        emitter_page = emitter_allocate_new_page(page_size, emitter->last_page);
        emitter->last_page = emitter_page;
        emitter->capacity += page_size;
        if (emitter->first_page == 0) {
            // we just have created first page for emitter
            emitter->first_page = emitter_page;
        }

        if (emitter->size + page_size == emitter->capacity) {
            // opcode and data can be fully written on the new last page
            goto normal_write;
        }
        size_t prev_free_space = emitter->capacity - emitter->size - page_size;
        size_t next_free_space = n - prev_free_space;
        ubyte_t *prev_ptr = (ubyte_t *) (prev_emitter_page + page_size - prev_free_space);
        // write first part to the previous page
        memcpy(prev_ptr, data, prev_free_space);
        // write second part to the new page
        memcpy(emitter_page, data + prev_free_space, next_free_space);
        emitter->size += n;
        return n;
    }
    normal_write:
    // lets find a place to write opcode and data
    emitter_page = emitter->last_page;
    size_t offset = emitter->size % emitter->page_size;
    ubyte_t *data_dst = (ubyte_t *) (emitter_page + offset);
    memcpy(data_dst, data, n);
    //printf("normal emit to page at %p \n",emitter_page);
    //printf("emitted to buffer %d bytes\n", n + 1);
    emitter->size += n;
    return n;
}

size_t emitter_emit_raw(struct bytecode_emitter_t *emitter, size_t n, void *data) {
    //printf(" raw emit : ");
    if (emitter->page_size == INT32_MAX) {
        // unbuffered
        vm_execute_opcodes(n, data);
        return n;
    }
    size_t n_left = n;
    size_t page_size = emitter->page_size;
    if (!emitter->first_page) {
        // if we are emitting to empty emitter
        void *page = emitter_allocate_new_page(page_size, 0);
        emitter->first_page = page;
        emitter->last_page = page;
        emitter->capacity += page_size;
    }
    void *current_data = data;
    void *current_page = emitter->last_page;

    raw_write:;
    size_t current_page_offset = emitter->size % page_size;
    size_t current_page_left = page_size - current_page_offset;
    size_t size_to_write = n_left > current_page_left ? current_page_left : n_left;
    memcpy(current_page + current_page_offset, current_data, size_to_write);
    n_left -= size_to_write;
    current_data += size_to_write;
    emitter->size += size_to_write;
    if (n_left > 0) {
        void *page = emitter_allocate_new_page(page_size, current_page);
        emitter->last_page = page;
        emitter->capacity += page_size;
        current_page = page;
        goto raw_write;
    }
    return n;
}

void compiler_emitter_buffered_destroy(struct bytecode_emitter_t *emitter) {
    size_t page_size = emitter->page_size;
    void *current_page = emitter->first_page;
    while (current_page) {
        void **next_pp = (void **) (current_page + page_size);
        void *next_ptr = *next_pp;
        free(current_page);
        current_page = next_ptr;
    }
}

size_t compiler_emitter_sink(struct bytecode_emitter_t *emitter, void *dst, size_t max_size) {
    size_t page_size = emitter->page_size;
    size_t size = emitter->size;
    if (size == 0) {
        return 0;
    }
    void *out_data = dst;//malloc(size);
    void *current_page = emitter->first_page;
    void *current_data = out_data;
    //TODO: do not ignore max_size
    size_t bytes_left = size;
    while (bytes_left > 0) {
        uint8_t current_page_data[page_size + sizeof(address_t)];
        memcpy(current_page_data, current_page, page_size + sizeof(address_t));
        memcpy(current_data, current_page, bytes_left > page_size ? page_size : bytes_left);
        void **next_ptr = (void **) (current_page + page_size);
        void *next_page = *next_ptr;
        current_data += page_size;
        current_page = next_page;
        bytes_left -= page_size > bytes_left ? bytes_left : page_size;
    }
    return max_size;
}

void compiler_add_commentf(struct scope_handler_t *scope, const char *format, ...) {
    UNUSED(scope)
    UNUSED(format)
    /*va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);*/
}

struct bytecode_emitter_t compiler_emitter_buffered_new(size_t page_size) {
    struct bytecode_emitter_t emitter;
    //emitter.raw_emitter_func = emitter_emit_raw;
    emitter.emitter_func = emitter_emit_buffered;
    emitter.page_size = page_size;
    emitter.capacity = 0;
    emitter.size = 0;
    emitter.first_page = 0;
    emitter.last_page = 0;
    return emitter;
}


static inline int compiler_emit_n_impl(struct scope_handler_t *scope, /*opcode_t opcode,*/ size_t len, void *data) {
    //printf("%s:%d: emit impl \n", __FILE__, __LINE__);
    ubyte_t array[len];
    memcpy(array, data, len);
    return (int) scope->emitter->emitter_func(scope->emitter, len, data);
}

int compiler_emit_0(struct scope_handler_t *scope, opcode_t opcode) {
    uint8_t data[1];
    memcpy(data + 0, &opcode, 1);
    //compiler_add_commentf(scope, "\033[32m op[0] : %s;\033[0m\n", get_opcode_mnemonic(opcode));
    compiler_emit_n_impl(scope, 1, data);
    return 0;
}

int compiler_emit_1(struct scope_handler_t *scope, opcode_t opcode, ubyte_t byte1) {
    uint8_t data[2];
    memcpy(data + 0, &opcode, 1);
    memcpy(data + 1, &byte1, 1);
    //compiler_add_commentf(scope, "\033[32m op[1] : %s %u;\033[0m\n", get_opcode_mnemonic(opcode), byte1);
    compiler_emit_n_impl(scope, 2, &data);
    return 0;
}

int compiler_emit_2(struct scope_handler_t *scope, opcode_t opcode, ubyte_t byte1, ubyte_t byte2) {
    //compiler_add_commentf(scope, "\033[32m op[2] : %s %u %u;\033[0m\n", get_opcode_mnemonic(opcode), byte1, byte2);
    uint8_t data[3];
    memcpy(data + 0, &opcode, 1);
    memcpy(data + 1, &byte1, 1);
    memcpy(data + 2, &byte2, 1);
    compiler_emit_n_impl(scope, 3, data);
    return 0;
}

int compiler_emit_n(struct scope_handler_t *scope, size_t len, void *data) {
    //compiler_add_commentf(scope, "\033[32m op[%zu] : %s;\033[0m\n", len, get_opcode_mnemonic(opcode));
    ubyte_t array[len];
    memcpy(array, data, len);
    return compiler_emit_n_impl(scope, len, data);
}
