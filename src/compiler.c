/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include "parser.h"
#include "vm/opcodes.h"
#include "compiler.h"

struct execution_queue_t node_queue = {
        .cap = 256,
        .size = 0
};

int compile_queue() {
    for (size_t i = 0; i < node_queue.size; i++) {
        int ret = compiler_compile(node_queue.q[node_queue.execution]);
        if (ret) {
            return ret;
        }
        node_queue.execution = (node_queue.execution + 1) % node_queue.cap;
    }
    return COMPILE_SUCCESS;
}


int compiler_pop_and_compile(ast_node_t *node) {
    if (node_queue.size != 0) {
        compile_queue();
    }
    return compiler_compile(node);
}

int compiler_push(ast_node_t *node) {
    if (node_queue.size == node_queue.cap) {
        return COMPILE_NODE_QUEUE_OVERFLOW;
    }
    node_queue.q[node_queue.free] = node;
    node_queue.free = (node_queue.free + 1) % node_queue.cap;
    ++node_queue.size;
    return COMPILE_SUCCESS;
}

int compile_global_statement(ast_node_stat_t *node) {
    return compile_statement(compiler_get_root_frame(), node);
}

int compile_global_func(ast_node_decl_func_t *node) {
    if (node->type == AST_NODE_TYPE_DECL_FUNC) {

    }
    compiler_panic("unable to compile global func node");
}

int compiler_compile_impl(ast_node_t *node) {
    // root statement pushed within global scope
    if ((node->type & AST_NODE_STAT_MASK) >> 6u == 0b10u) {
        return compile_global_statement((ast_node_stat_t *) node);
    }
    // function declaration in global scope
    if (node->type == AST_NODE_TYPE_DECL_FUNC) {
        return compile_global_func((ast_node_decl_func_t *) node);
    }
    return COMPILE_UNEXPECTED_NODE;
}

int compiler_compile(ast_node_t *node) {
    struct timeval interval_1;
    struct timeval interval_2;
    gettimeofday(&interval_1, 0);
    int res = compiler_compile_impl(node);
    gettimeofday(&interval_2, 0);
    long start_time = interval_1.tv_sec * (int) 1e6 + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * (int) 1e6 + interval_2.tv_usec;
    printf("compilation finished in %ld microseconds : %d\n", (end_time - start_time), res);
    return res;
}

int compiler_finish() {
    return compile_queue();
}

void compiler_panic(const char *format, ...) {
    printf("Compiler error: ");
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
    exit(255);
}