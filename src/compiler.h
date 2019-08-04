/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_COMPILER_H
#define SHELL_COMPILER_H

#include <string.h>
#include "parser.h"
#include "types.h"
#include "opcodes.h"

#define NODE_COMPILED(node) (( node->flags & 0b10000000u ) >> 6u == 1)

#define COMPILE_SUCCESS             0x00
#define COMPILE_UNEXPECTED_NODE     0x80
#define COMPILE_NODE_QUEUE_OVERFLOW 0x81

struct execution_queue_t {
    size_t cap, size;
    size_t execution, free;
    struct ast_node_t *q[256];
};

struct scope_var_t {
    unsigned char type_id;
    string_t type;
    string_t name;
    ubyte_t index;
};

struct stack_var_t {
    ubyte_t type;
    ubyte_t size;
    union {
        bool_t bool_val;
        int_t int_val;
        float_t float_val;
        char_t char_val;
        string_t string_val;
    };
};

struct scope_handler_t {
    struct scope_handler_t *parent;
    struct {
        size_t stack_cap, stack_size;
        struct stack_var_t *stack;
    };
    struct {
        size_t vars_size, vars_cap;
        struct scope_var_t *vars;
    };
};

struct cast_request_t {
    opcode_t opcode;
    ubyte_t var_index;
    ubyte_t to_type;
};

int compile_expression(struct scope_handler_t *scope, ast_node_expr_t *expr_node);

int compiler_emit_0(opcode_t opcode);

int compiler_emit_1(opcode_t opcode, ubyte_t byte1);

int compiler_emit_2(opcode_t opcode, ubyte_t byte1, ubyte_t byte2);

int compiler_emit_n(opcode_t opcode, size_t len, void *data);

struct scope_handler_t *compiler_get_root_scope();

const struct scope_var_t *scope_lookup_var_by_name(struct scope_handler_t *scope,
                                                   const char *name, bool_t use_parents);

const struct scope_var_t *scope_define_var(struct scope_handler_t *scope, ubyte_t type_id,
                                           const char *type, const char *name);

int ast_node_trace_data_type(ast_node_expr_t *node);

#endif //SHELL_COMPILER_H
