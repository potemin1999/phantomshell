/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <endian.h>
#include <vm/vm.h>
#include <malloc.h>
#include "compiler.h"
#include "parser.h"
#include "lexer.h"

int compile_global_statement(ast_node_stat_t *node);

int compile_global_func(ast_node_decl_func_t *node);

int compile_literal_int(struct scope_handler_t *scope, ast_node_literal_t *liter_node) {
    uint8_t data[5];
    uint32_t h_int_val = (uint32_t) liter_node->int_val;
    uint32_t be_int_val = htobe32(h_int_val);
    data[0] = OPCODE_ICONST;
    memcpy(data + 1, &be_int_val, 4);
    compiler_emit_n(scope, 5, &data);
    return 0;
}

int compile_literal_float(struct scope_handler_t *scope, ast_node_literal_t *liter_node) {
    uint8_t data[5];
    data[0] = OPCODE_FCONST;
    memcpy(data + 1, &(liter_node->float_val), 4);
    compiler_emit_n(scope, 5, &data);
    return 0;
}

int compile_ident(struct scope_handler_t *scope, ast_node_ident_t *ident_node) {
    const struct scope_var_t *var = frame_lookup_var_by_name(scope, ident_node->value, 1);
    if (var) {
        opcode_t opcode;
        OPCODE_LOAD(var->static_type, &opcode)
        compiler_emit_1(scope, opcode, var->index);
        ident_node->flags &= 0x80u;
        return 0;
    } else {
        return 1;
    }
}

int compile_group(struct scope_handler_t *scope, ast_node_group_t *group_node) {
    return compile_expression(scope, (ast_node_expr_t *) group_node->expr);
}

int compile_unary_op(struct scope_handler_t *scope, ast_node_unary_op_t *expr_node) {
    int ret = 0;
    ast_node_expr_t *operand = expr_node->operand;
    unsigned char opcode = int_operator_to_opcode(expr_node->operator);
    if (opcode == 0) {
        compiler_panic("unable to compile unary op");
    } else {
        // operator will be executed on the stack
        if ((ret = compile_expression(scope, operand))) {
            return ret;
        }
        compiler_emit_0(scope, opcode);
    }
    return 0;
}

int compile_binary_op_assignment(struct scope_handler_t *scope, ast_node_binary_op_t *expr_node,
                                 ast_node_expr_t *l, ast_node_expr_t *r) {
    UNUSED(expr_node)
    int ret = 0;
    opcode_t opcode = 0;
    opcode_t cast_opcode = 0;
    static_type_t var_type = 0;
    const struct scope_var_t *var = 0;
    int res = ast_node_trace_data_type(scope, r);
    if (res) {
        compiler_panic("unable to determine node type");
    }
    static_type_t r_type = EXPR_NODE_STATIC_TYPE(r);
    ast_node_ident_t *l_ident = (ast_node_ident_t *) l;
    var = frame_lookup_var_by_name(scope, l_ident->value, 1);

    if (var && var->static_type != r_type) {
        // var was defined and type cast is required
        struct cast_request_t cr = type_select_cast_method(var->static_type, r_type);
        if (cr.var_index == 0) {
            //TODO: make explicit casting
            compiler_panic("assignment error: cast request requires explicit casting");
        }
        cast_opcode = cr.opcode;
        var_type = cr.static_type;
    } else if (var && var->static_type == r_type) {
        // var was defined and the type casting is not required
        var_type = r_type;
    } else {
        // var was not defined
        var = frame_define_var(scope, r_type, l_ident->value);
        var_type = r_type;
    }

    OPCODE_SAVE(var_type, &opcode)
    ubyte_t index = var->index;
    if ((ret = compile_expression(scope, r)) != 0) {
        return ret;
    }
    if (cast_opcode) {
        compiler_emit_0(scope, cast_opcode);
    }
    compiler_emit_1(scope, opcode, index);
    return 0;
}

int compile_function_call(struct scope_handler_t *scope, ast_node_binary_op_t *expr_node,
                          ast_node_expr_t *l, ast_node_expr_t *r) {
    if (l->type == AST_NODE_TYPE_IDENT) {
        opcode_t opcode = OPCODE_CALL;
        ast_node_ident_t *ident = (ast_node_ident_t *) l;
        const struct func_desc_t *func_desc = 0;
        func_desc = compiler_lookup_func_by_name(ident->value);
        if (!func_desc) {
            compiler_panic("function %s not found", ident->value);
        }
        uint16_t func_sig_index = 0;
        const char *func_signature = func_desc->signature;
        const_pool_index_by_value(func_signature, &func_sig_index);

        if (r->type != AST_NODE_TYPE_GROUP) {
            compiler_panic("group was expected as the right part of the function call operator");
        }
        ast_node_group_t *r_group = (ast_node_group_t *) r;
        ast_node_expr_t *current_arg = r_group->expr;
        while (current_arg) {
            if (current_arg->type == AST_NODE_TYPE_BINARY_OP) {
                ast_node_binary_op_t *current_arg_bin = (ast_node_binary_op_t *) current_arg;
                if (current_arg_bin->operator == COMMA) {
                    printf("%s:%d : compiling comma expression\n", __FILE__, __LINE__);
                    compile_expression(scope, current_arg_bin->left);
                    current_arg = current_arg_bin->right;
                    continue;
                } else {
                    compiler_panic("unexpected binary operator in function args");
                }
            }
            printf("%s:%d : compiling expression\n", __FILE__, __LINE__);
            compile_expression(scope, current_arg);
            break;
        }
        printf("%s:%d : call compiled, writing opcode\n", __FILE__, __LINE__);
        compiler_emit_2(scope, opcode, func_sig_index >> 8u, (ubyte_t) func_sig_index);
        printf("call compiled, writed opcode\n");
        return 0;
    } else {
        compiler_panic("dont know how to build member function call yet");
    }
}

int compile_binary_op(struct scope_handler_t *scope, ast_node_binary_op_t *expr_node) {
    ast_node_expr_t *r = expr_node->right;
    ast_node_expr_t *l = expr_node->left;
    if (expr_node->operator == ASSIGNMENT) {
        // assingment operation is opcoded differently
        return compile_binary_op_assignment(scope, expr_node, l, r);
    }
    if (expr_node->operator == FUNCTION_CALL) {
        // call function of the object or the static one
        return compile_function_call(scope, expr_node, l, r);
    }
    unsigned char opcode = 0;
    ast_node_trace_data_type(scope, (ast_node_expr_t *) expr_node);
    switch (expr_node->static_type) {
        case TYPE_INT: {
            opcode = int_operator_to_opcode(expr_node->operator);
            break;
        }
        case TYPE_FLOAT: {
            opcode = float_operator_to_opcode(expr_node->operator);
            break;
        }
        default: {
            //compiler_panic("unable to handle binary op with type %d",expr_node->static_type);
        }
    }
    if (opcode == 0) {
        // no suitable opcode was found
        compiler_panic("binary op compilation failed : no suitable opcode was found");
    } else {
        // typical binary operator
        int ret = 0;
        if ((ret = compile_expression(scope, l))) {
            return ret;
        }
        if ((ret = compile_expression(scope, r))) {
            return ret;
        }
        compiler_emit_0(scope, opcode);
        return 0;
    }
}

int compile_special_cast(struct scope_handler_t *scope, ast_node_special_cast_t *expr_node) {
    int ret;
    if ((ret = compile_expression(scope, expr_node->operand))) {
        return ret;
    }
    compiler_emit_0(scope, expr_node->opcode);
    return 0;
}

int compile_expression(struct scope_handler_t *scope, ast_node_expr_t *expr_node) {
    switch (expr_node->type) {
        case AST_NODE_TYPE_LITERAL_INT: return compile_literal_int(scope, (ast_node_literal_t *) expr_node);
        case AST_NODE_TYPE_LITERAL_FLOAT: return compile_literal_float(scope, (ast_node_literal_t *) expr_node);
        case AST_NODE_TYPE_IDENT: return compile_ident(scope, (ast_node_ident_t *) expr_node);
        case AST_NODE_TYPE_GROUP: return compile_group(scope, (ast_node_group_t *) expr_node);
        case AST_NODE_TYPE_UNARY_OP: return compile_unary_op(scope, (ast_node_unary_op_t *) expr_node);
        case AST_NODE_TYPE_BINARY_OP: return compile_binary_op(scope, (ast_node_binary_op_t *) expr_node);
        case AST_NODE_TYPE_SPECIAL_CAST: return compile_special_cast(scope, (ast_node_special_cast_t *) expr_node);
        default: compiler_panic("unknown type of expression");
    }
}