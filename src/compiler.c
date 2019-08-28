/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "lib.h"
#include "parser.h"
#include "compiler.h"

struct program_file_t *current_file;

struct program_file_t *complier_get_current_file() {
    return current_file;
}


int compiler_compile_impl(ast_node_t *node) {
    struct scope_handler_t *scope = compiler_get_root_frame();
    if ((node->type & AST_NODE_STAT_MASK) >> 6U == 0b10U) {
        // root statement pushed within global scope
        return compile_statement(scope, (ast_node_stat_t *) node);
    }
    if (node->type == AST_NODE_TYPE_DECL_FUNC) {
        // function declaration in global scope
        return compile_func(scope, (ast_node_decl_func_t *) node);
    }
    return COMPILE_UNEXPECTED_NODE;
}

int compiler_compile(ast_node_t *node) {
    struct timeval interval_1;
    struct timeval interval_2;
    gettimeofday(&interval_1, 0);
    int res = compiler_compile_impl(node);
    gettimeofday(&interval_2, 0);
    const int usec_mult = 1000000;
    long start_time = interval_1.tv_sec * usec_mult + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * usec_mult + interval_2.tv_usec;
    printf("compilation finished in %ld microseconds : %d\n", (end_time - start_time), res);
    return res;
}

int compiler_init() {
    current_file = malloc(sizeof(struct program_file_t));
    current_file->package = "";
    current_file->functions_map = hashmap_new();
    compiler_init_builtins();
    return 0;
}

int compiler_finish() {
    return 0;
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

const int32_t opcode_to_stack_inc_map[OPCODES_COUNT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0x00 - 0x0f]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0x10 - 0x1f]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0x20 - 0x2f]
        0, 0, 4, -4, 4, -4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0,          // [0x30 - 0x3f]
        0, 0, -4, -4, -4, -4, 0, 0, 0, 0, -4, -4, -4, -4, -4, -4,  // [0x40 - 0x4f]
        -4, -4, -4, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        // [0x50 - 0x5f]
        -2, -2, -4, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        // [0x60 - 0x6f]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0x70 - 0x7f]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0x80 - 0x8f]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0x90 - 0x9f]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0xa0 - 0xaf]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0xb0 - 0xbf]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0xc0 - 0xcf]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0xd0 - 0xdf]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0xe0 - 0xef]
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,            // [0xf0 - 0xff]
};
