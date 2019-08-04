/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "compiler.h"
#include "parser.h"

int compile_root_statement(ast_node_stat_t *node);

int compile_root_func(ast_node_decl_func_t *node);

int compile_literal_int(struct scope_handler_t *scope, ast_node_literal_t *liter_node) {
    switch (liter_node->type) {
        case AST_NODE_TYPE_LITERAL_INT: {
            compiler_emit_n(OPCODE_ICONST, 4, &(liter_node->int_val));
            return 0;
        }
        default: {
            return 34;
        }
    }
}

int compile_ident(struct scope_handler_t *scope, ast_node_ident_t *ident_node) {
    const struct scope_var_t *var = scope_lookup_var_by_name(scope, ident_node->value, 1);
    if (var) {
        compiler_emit_1(OPCODE_ILOAD, var->index);
        ident_node->flags &= 0x80u;
        return 0;
    } else {
        return 1;
    }
}

int compile_binary_op(struct scope_handler_t *scope, ast_node_binary_op_t *expr_node) {
    int ret = 0;
    ast_node_expr_t *r = expr_node->right;
    ast_node_expr_t *l = expr_node->left;

    unsigned char opcode = int_operator_to_opcode(expr_node->operator);
    if (opcode == 0) {
        // no suitable opcode was found
        if (expr_node->operator == ASSIGNMENT) {
            int res = ast_node_trace_data_type(r);
            if (res) {
                printf("unable to determine node type");
            }
            ubyte_t r_type = EXPR_NODE_TYPE(r);
            OPCODE_SAVE(r_type, &opcode);
            ast_node_ident_t *l_ident = (ast_node_ident_t *) l;
            const struct scope_var_t *var = 0;
            var = scope_lookup_var_by_name(scope, l_ident->value, 1);
            if (!var) {
                var = scope_define_var(scope, EXPR_NODE_TYPE(expr_node),
                                       "", strdup(l_ident->value));
            }
            ubyte_t index = var->index;
            if (!NODE_COMPILED(r)) {
                if ((ret = compile_expression(scope, r)) != 0) goto error;
            }
            compiler_emit_1(opcode, index);
        }
    } else {
        // typical binary operator
        if (!NODE_COMPILED(r)) {
            if ((ret = compile_expression(scope, r)) != 0) goto error;
        }
        if (!NODE_COMPILED(l)) {
            if ((ret = compile_expression(scope, l)) != 0) goto error;
        }
        compiler_emit_0(opcode);
    }
    return 0;
    error:
    return ret;
}

int compile_expression(struct scope_handler_t *scope, ast_node_expr_t *expr_node) {
    switch (expr_node->type) {
        case AST_NODE_TYPE_LITERAL_INT:return compile_literal_int(scope, (ast_node_literal_t *) expr_node);
        case AST_NODE_TYPE_IDENT:return compile_ident(scope, (ast_node_ident_t *) expr_node);
        case AST_NODE_TYPE_BINARY_OP:return compile_binary_op(scope, (ast_node_binary_op_t *) expr_node);
    }
    return 22;
}