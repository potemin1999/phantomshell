/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/6/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdlib.h>
#include "compiler.h"
#include "lexer.h"

int compile_ret_statement(struct scope_handler_t *scope, ast_node_stat_if_t *if_node) {
    UNUSED(scope)
    UNUSED(if_node)
    compiler_panic("compilation of ret statement is not yet implemented");
}

int compile_if_statement(struct scope_handler_t *scope, ast_node_stat_if_t *if_node) {
    struct bytecode_emitter_t true_emitter = compiler_emitter_buffered_new(32);
    struct bytecode_emitter_t false_emitter = compiler_emitter_buffered_new(32);
    compile_expression(scope, (ast_node_expr_t *) if_node->expr);
    struct scope_handler_t true_scope;
    struct scope_var_t true_scope_vars[128];
    true_scope.vars = true_scope_vars;
    true_scope.vars_cap = scope->vars_cap;
    true_scope.vars_off = scope->vars_size + scope->vars_off;
    true_scope.vars_max_size = 0;
    true_scope.vars_size = 0;
    true_scope.parent = scope;
    true_scope.emitter = &true_emitter;
    true_scope.is_function_scope = 0;
    compile_scope(&true_scope, if_node->true_scope);
    //printf("true scope was compiled : %zu\n", true_emitter.size);
    struct scope_handler_t false_scope;
    struct scope_var_t false_scope_vars[128];
    false_scope.vars = false_scope_vars;
    false_scope.vars_cap = scope->vars_cap;
    false_scope.vars_off = scope->vars_size + scope->vars_off;
    true_scope.vars_max_size = 0;
    false_scope.vars_size = 0;
    false_scope.parent = scope;
    false_scope.emitter = &false_emitter;
    false_scope.is_function_scope = 0;
    compile_scope(&false_scope, if_node->false_scope);
    //printf("false scope was compiled : %zu\n", false_emitter.size);
    if (false_scope.emitter->size > 65536) {
        //TODO: handle long jump
        //uint32_t offset = false_scope.emitter->size;
        //true_emitter.emitter_func(&true_emitter, OPCODE_LJMP, 4, &offset);
    } else {
        uint16_t offset_raw = (uint16_t) false_scope.emitter->size;
        uint16_t offset_be = htobe16(offset_raw);
        true_emitter.emitter_func(&true_emitter, OPCODE_JMP, 2, &offset_be);
    }
    if (true_scope.emitter->size > 65536) {
        //TODO: handle long jump
    } else {
        size_t true_size = true_emitter.size;
        size_t false_size = false_emitter.size;
        ubyte_t hi = (ubyte_t) ((uint32_t) true_size) >> 8u;
        ubyte_t lo = (ubyte_t) ((uint32_t) true_size);
        ubyte_t *exec_data = (ubyte_t *) malloc(true_size + false_size + 3);
        exec_data[0] = OPCODE_JEZ;
        exec_data[1] = hi;
        exec_data[2] = lo;
        compiler_emitter_sink(&true_emitter, exec_data + 3, true_size);
        compiler_emitter_sink(&false_emitter, exec_data + 3 + true_size, false_size);
        //compiler_emit_2(scope, OPCODE_JEZ, hi,lo);
        scope->emitter->raw_emitter_func(scope->emitter, true_size + false_size + 3, exec_data);
        free(exec_data);
    }
    frame_destroy_scope(&true_scope);
    frame_destroy_scope(&false_scope);
    compiler_emitter_buffered_free(&true_emitter);
    compiler_emitter_buffered_free(&false_emitter);
    return 0;
}

int compile_statement(struct scope_handler_t *scope, ast_node_stat_t *stat_node) {
    switch (stat_node->type) {
        case AST_NODE_TYPE_STAT_EXPR: return compile_expression(scope, ((ast_node_stat_expr_t *) stat_node)->expr);
        case AST_NODE_TYPE_STAT_IF: return compile_if_statement(scope, (ast_node_stat_if_t *) stat_node);
        default: compiler_panic("uncompilable type of statement");
    }
}