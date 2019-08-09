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
#include "vm/opcodes.h"

#define NODE_COMPILED(node) (( (node)->flags & 0b10000000u ) >> 6u == 1)

#define COMPILE_SUCCESS             0x00
#define COMPILE_UNEXPECTED_NODE     0x80
#define COMPILE_NODE_QUEUE_OVERFLOW 0x81

struct execution_queue_t {
    size_t cap, size;
    size_t execution, free;
    struct ast_node_t *q[256];
};

struct scope_var_t {
    uint64_t static_type;
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

struct bytecode_emitter_t;

struct scope_handler_t {
    struct scope_handler_t *parent;
    uint8_t is_function_scope;
    struct {
        size_t stack_cap, stack_size;
        size_t stack_max_size;
        struct stack_var_t *stack;
    };
    struct {
        size_t vars_off;
        size_t vars_size, vars_cap;
        size_t vars_max_size;
        struct scope_var_t *vars;
    };
    struct bytecode_emitter_t *emitter;
};

struct cast_request_t {
    opcode_t opcode;
    ubyte_t var_index;
    static_type_t static_type;
};

/**
 * Executes ready statement/function declaration/class definition
 * @param node
 * @return execution code
 */

int compiler_release();

int compiler_init();

int compiler_pop_and_compile(ast_node_t *node);

int compiler_push(ast_node_t *node);

int compiler_compile(ast_node_t *node);

int compiler_finish(ast_node_t *node);

void compiler_add_commentf(struct scope_handler_t *scope, const char *format, ...);

void compiler_panic(const char *str, ...) __attribute__ ((__noreturn__));


int compiler_emit_0(struct scope_handler_t *scope, opcode_t opcode);

int compiler_emit_1(struct scope_handler_t *scope, opcode_t opcode, ubyte_t byte1);

int compiler_emit_2(struct scope_handler_t *scope, opcode_t opcode, ubyte_t byte1, ubyte_t byte2);

int compiler_emit_n(struct scope_handler_t *scope, opcode_t opcode, size_t len, void *data);


int compile_expression(struct scope_handler_t *scope, ast_node_expr_t *expr_node);

int compile_scope(struct scope_handler_t *new_scope, ast_node_scope_t *scope_node);

int compile_statement(struct scope_handler_t *scope, ast_node_stat_t *stat_node);

int compile_func(struct scope_handler_t *scope, ast_node_decl_func_t *func_node);


struct scope_handler_t *compiler_get_root_frame();

const struct scope_var_t *frame_lookup_var_by_name(struct scope_handler_t *scope,
                                                   const char *name, bool_t use_parents);

const struct scope_var_t *frame_define_var(struct scope_handler_t *scope,
                                           static_type_t type_id, const char *name);

void frame_init_scope(struct scope_handler_t *scope);

void frame_destroy_scope(struct scope_handler_t *scope);

// Vars
struct cast_request_t type_select_cast_method(static_type_t type_1, static_type_t type_2);

int ast_node_trace_data_type(struct scope_handler_t *scope, ast_node_expr_t *node);

// Emitters

struct bytecode_emitter_t {
    size_t (*raw_emitter_func)(struct bytecode_emitter_t *emitter, size_t n, void *data);

    size_t (*emitter_func)(struct bytecode_emitter_t *emitter, opcode_t opcode, size_t n, void *data);

    ubyte_t *first_page;
    ubyte_t *last_page;
    size_t size;
    size_t capacity;
    size_t page_size;
};

struct bytecode_emitter_t compiler_emitter_unbuffered_new();

struct bytecode_emitter_t compiler_emitter_buffered_new(size_t page_size);

void compiler_emitter_buffered_free(struct bytecode_emitter_t *emitter);

size_t compiler_emitter_sink(struct bytecode_emitter_t *emitter, void *dst, size_t max_size);

#endif //SHELL_COMPILER_H
