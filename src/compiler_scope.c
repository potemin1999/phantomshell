/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */
#include "compiler.h"

struct stack_var_t root_handler_stack[512];
struct scope_var_t root_scope_vars[64];

struct scope_handler_t root_scope_handler = {
        .parent = 0,
        .stack_size = 0,
        .stack_cap = 512,
        .stack = root_handler_stack,
        .vars_size = 0,
        .vars_cap = 64,
        .vars = root_scope_vars
};

struct scope_handler_t *compiler_get_root_scope() {
    return &root_scope_handler;
}

ubyte_t stack_top_type(struct scope_handler_t *scope) {
    return scope->stack[scope->stack_size].type;
}

const struct scope_var_t *scope_lookup_var_by_name(struct scope_handler_t *scope,
                                                   const char *name, bool_t use_parents) {
    for (size_t i = 0; i < scope->vars_size; i++) {
        if (strcmp(scope->vars[i].name, name) == 0) {
            return &(scope->vars[i]);
        }
    }
    if (use_parents && scope->parent) {
        return scope_lookup_var_by_name(scope->parent, name, use_parents);
    }
    return 0;
}

const struct scope_var_t *
scope_define_var(struct scope_handler_t *scope, ubyte_t type_id, const char *type, const char *name) {
    struct scope_var_t *var = &(scope->vars[scope->vars_size]);
    var->name = strdup(name);
    var->type = strdup(type);
    var->type_id = type_id;
    var->index = scope->vars_size;
    scope->vars_size++;
    return var;
}

