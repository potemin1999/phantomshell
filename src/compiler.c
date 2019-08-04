/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "opcodes.h"
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

int compile_root_statement(ast_node_stat_t *node) {
    if (node->type == AST_NODE_TYPE_STAT_EXPR) {
        ast_node_stat_expr_t *stat_node = (ast_node_stat_expr_t *) node;
        return compile_expression(compiler_get_root_scope(), (ast_node_expr_t *) stat_node->expr);
    }
    return 2;
}

int compile_root_func(ast_node_decl_func_t *node) {
    return 3;
}

int compiler_compile(ast_node_t *node) {
    // root statement pushed within global scope
    if ((node->type & AST_NODE_STAT_MASK) >> 6u == 0b10u) {
        return compile_root_statement((ast_node_stat_t *) node);
    }
    // function declaration in global scope
    if (node->type == AST_NODE_TYPE_DECL_FUNC) {
        return compile_root_func((ast_node_decl_func_t *) node);
    }
    return COMPILE_UNEXPECTED_NODE;
}

int compiler_finish(ast_node_t *node) {
    return compile_queue();
}

int compiler_emit_0(opcode_t opcode) {
    printf(" op[0] : %s;\n", get_opcode_name(opcode));
    return 0;
}

int compiler_emit_1(opcode_t opcode, ubyte_t byte1) {
    printf(" op[1] : %s %u;\n", get_opcode_name(opcode), byte1);
    return 0;
}

int compiler_emit_2(opcode_t opcode, ubyte_t byte1, ubyte_t byte2) {
    printf(" op[2] : %s %u %u;\n", get_opcode_name(opcode), byte1, byte2);
    return 0;
}

int compiler_emit_n(opcode_t opcode, size_t len, void *data) {
    printf(" op[%zu] : %s;\n", len, get_opcode_name(opcode));
    return 0;
}