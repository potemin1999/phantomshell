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
#include "vm/opcodes.h"
#include "compiler.h"

struct execution_queue_t node_queue = {
        .cap = 256,
        .size = 0
};

struct program_file_t *current_file;

struct program_file_t *complier_get_current_file() {
    return current_file;
}


int compiler_compile_impl(ast_node_t *node) {
    struct scope_handler_t *scope = compiler_get_root_frame();
    if ((node->type & AST_NODE_STAT_MASK) >> 6u == 0b10u) {
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
    long start_time = interval_1.tv_sec * (int) 1e6 + interval_1.tv_usec;
    long end_time = interval_2.tv_sec * (int) 1e6 + interval_2.tv_usec;
    printf("compilation finished in %ld microseconds : %d\n", (end_time - start_time), res);
    return res;
}

int compiler_init() {
    current_file = malloc(sizeof(struct program_file_t));
    current_file->package = "";
    current_file->functions_map = hashmap_new();
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