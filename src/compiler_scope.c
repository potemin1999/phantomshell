/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include <stdlib.h>
#include "compiler.h"

struct stack_var_t root_handler_stack[512];
struct scope_var_t root_scope_vars[64];

struct scope_handler_t root_scope_handler = {
        .parent = 0,
        .emitter = 0,
        .is_function_scope = 0,
        .stack_size = 0,
        .stack_cap = 512,
        .stack_max_size = 0,
        .stack = root_handler_stack,
        .vars_off = 0,
        .vars_size = 0,
        .vars_max_size = 0,
        .vars_cap = 64,
        .vars = root_scope_vars
};

struct scope_handler_t *compiler_get_root_frame() {
    if (!root_scope_handler.emitter) {
        struct bytecode_emitter_t *emitter_copy;
        struct bytecode_emitter_t emitter = compiler_emitter_unbuffered_new();
        emitter_copy = (struct bytecode_emitter_t *) malloc(sizeof(struct bytecode_emitter_t));
        memcpy(emitter_copy, &emitter, sizeof(struct bytecode_emitter_t));
        root_scope_handler.emitter = emitter_copy;
    }
    return &root_scope_handler;
}

void frame_init_scope(struct scope_handler_t *scope) {
    scope->parent = 0;
    scope->is_function_scope = 0;
    scope->stack = 0;
    scope->stack_cap = 0;
    scope->stack_size = 0;
    scope->stack_max_size = 0;
    scope->vars = 0;
    scope->vars_off = 0;
    scope->vars_cap = 0;
    scope->vars_size = 0;
    scope->vars_max_size = 0;
}

void frame_destroy_scope(struct scope_handler_t *scope) {
    if (scope->vars) {
        for (size_t i = scope->vars_off; i < scope->vars_off + scope->vars_size; i++) {
            struct scope_var_t *var = (scope->vars + i);
            free(var->name);
        }
    }
}

ubyte_t stack_top_type(struct scope_handler_t *scope) {
    return scope->stack[scope->stack_size].type;
}

const struct scope_var_t *frame_lookup_var_by_name(struct scope_handler_t *scope,
                                                   const char *name, bool_t use_parents) {
    for (size_t i = scope->vars_off; i < scope->vars_size + scope->vars_off; i++) {
        struct scope_var_t *var = &(scope->vars[i]);
        if (strcmp(var->name, name) == 0) {
            return &(scope->vars[i]);
        }
    }
    if (use_parents && scope->parent && !scope->is_function_scope) {
        return frame_lookup_var_by_name(scope->parent, name, use_parents);
    }
    return 0;
}

const struct scope_var_t *frame_define_var(struct scope_handler_t *scope, static_type_t type_id, const char *name) {
    if (scope->vars_size >= scope->vars_cap) {
        compiler_panic("no free space left for new scope variables");
    }
    ubyte_t index = (ubyte_t) (scope->vars_size + scope->vars_off);
    struct scope_var_t *var = &(scope->vars[index]);
    var->name = strdup(name);
    var->static_type = type_id;
    var->index = index;
    scope->vars_size++;
    struct scope_handler_t *holding_scope = scope;
    while (holding_scope->parent && !holding_scope->is_function_scope) {
        holding_scope = holding_scope->parent;
    }
    holding_scope->vars_max_size++;
    return var;
}

int compile_scope(struct scope_handler_t *new_scope, ast_node_scope_t *scope_node) {
    if (!scope_node) {
        return 0;
    }
    ast_node_stat_list_t *stats = scope_node->stats;
    ast_node_stat_t *current_stat = stats->first;
    while (current_stat) {
        compile_statement(new_scope, current_stat);
        current_stat = (ast_node_stat_t *) current_stat->next;
    }
    return 0;
}
