/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "compiler.h"
#include "vm/opcodes.h"
#include "vm/runtime.h"

uint16_t const_pool_map_size = 0;
map_t const_pool_map;

static_type_t static_type_by_name(const char *type_name) {
    if (strcmp(type_name, "int") == 0) {
        return TYPE_INT;
    }
    if (strcmp(type_name, "float") == 0) {
        return TYPE_FLOAT;
    }
    if (strcmp(type_name, "void") == 0) {
        return TYPE_NONE;
    }
    compiler_panic("unknown static name type %s", type_name);
}

char *type_signature_by_name(const char *type_name) {
    if (strcmp(type_name, "int") == 0) {
        return strdup("I");
    }
    if (strcmp(type_name, "float") == 0) {
        return strdup("F");
    }
    if (strcmp(type_name, "void") == 0) {
        return strdup("V");
    }
    compiler_panic("unknown static name type %s", type_name);
}

int const_pool_index_by_value(const char *value, uint16_t *out_index) {
    if (!const_pool_map) {
        const_pool_map = hashmap_new();
    }
    void *void_value;
    int ret = hashmap_get(const_pool_map, value, &void_value);
    if (ret == 0) {
        memcpy(out_index, &void_value, 2);
        return 0;
    } else {
        return ret;
    }
}

int const_pool_register_value(const char *value, uint16_t *out_index) {
    if (!const_pool_map) {
        const_pool_map = hashmap_new();
    }
    //TODO: fix
    any_t void_value = 0;
    hashmap_get(const_pool_map, value, &void_value);
    if (void_value) {
        *out_index = *((uint16_t *) void_value);
        return 1;
    }
    memcpy(&void_value, &const_pool_map_size, 2);
    hashmap_put(const_pool_map, value, void_value);
    *out_index = const_pool_map_size;
    ++const_pool_map_size;
    return 0;
}

struct cast_request_t type_select_cast_method(static_type_t type_1, static_type_t type_2) {
#define TYPES_EQ(var1, var2, t1, t2) \
    ((((var1) == (t1))&&((var2)==(t2))) || (((var2)==(t1))&&((var1)==(t2))))

    if (TYPES_EQ(type_1, type_2, RUNTIME_TYPE_INT, RUNTIME_TYPE_FLOAT)) {
        struct cast_request_t cr = {
                .opcode = OPCODE_I2F,
                .static_type = RUNTIME_TYPE_FLOAT,
                .var_index = (ubyte_t) (type_1 == RUNTIME_TYPE_INT ? 0 : 1),
        };
        return cr;
    }
    struct cast_request_t cr = {
            .opcode = OPCODE_PANIC,
            .static_type = 0,
            .var_index = 0
    };
    return cr;
}

int ast_node_trace_binary_op_data_type(struct scope_handler_t *scope, ast_node_binary_op_t *binary_op) {
    ast_node_expr_t *left = binary_op->left;
    ast_node_expr_t *right = binary_op->right;
    if (binary_op->operator == FUNCTION_CALL) {
        // binary operator between identifier/member access operators and the group expression
        if (left->type == AST_NODE_TYPE_IDENT) {
            // global function call
            ast_node_ident_t *left_ident = (ast_node_ident_t *) left;
            map_t func_map = compiler_get_global_function_map();
            struct func_desc_t *func;
            int res = hashmap_get(func_map, left_ident->value, (any_t) &func);
            if (res) {
                //TODO: process errors
                compiler_panic("%s:%d function not found : %s", __LINE__, __FILE__, left_ident->value);
            }

            static_type_t type = static_type_by_name(func->ret_type);
            binary_op->left->static_type = type;
            binary_op->right->static_type = type;
            binary_op->static_type = type;
            return 0;
        } else {
            //TODO: trace member function return value type
            compiler_panic("unable to trace member function return value static type");
        }
    }
    if (EXPR_NODE_STATIC_TYPE(left) == TYPE_UNKNWN) {
        ast_node_trace_data_type(scope, left);
    }
    if (EXPR_NODE_STATIC_TYPE(right) == TYPE_UNKNWN) {
        ast_node_trace_data_type(scope, right);
    }
    static_type_t left_type = EXPR_NODE_STATIC_TYPE(left);
    static_type_t right_type = EXPR_NODE_STATIC_TYPE(right);
    static_type_t result_type = 0;
    if (left_type != right_type) {
        //printf("%s %d warn : cast not implemented %lu and %lu\n",
        //        __FILE__, __LINE__,left_type,right_type);
        struct cast_request_t cast_r = type_select_cast_method(left_type, right_type);
        ast_node_expr_t *operand = (cast_r.var_index == 0 ? left : right);
        ast_node_expr_t **operand_p = (cast_r.var_index == 0 ? &binary_op->left : &binary_op->right);
        *operand_p = (ast_node_expr_t *) ast_new_node_cast(operand, cast_r.static_type, cast_r.opcode);
        result_type = cast_r.static_type;
    } else {
        result_type = right_type;
    }
    if (IS_COMPARISON_OPERATOR(binary_op->operator)) {
        binary_op->static_type = RUNTIME_TYPE_BOOL;
    }
    binary_op->static_type = result_type;
    return 0;
}

int ast_node_trace_data_type(struct scope_handler_t *scope, ast_node_expr_t *node) {
    if (EXPR_NODE_STATIC_TYPE(node)) {
        return 0;
    }
    switch (node->type) {
        case AST_NODE_TYPE_IDENT: {
            ast_node_ident_t *ident_node = (ast_node_ident_t *) node;
            const struct scope_var_t *var = frame_lookup_var_by_name(scope, ident_node->value, 1);
            if (!var) {
                compiler_panic("unable to trace data type of undeclared/non-local variable %s", ident_node->value);
            }
            ident_node->static_type = var->static_type;
            return 0;
        }
        case AST_NODE_TYPE_GROUP: {
            ast_node_group_t *group_node = (ast_node_group_t *) node;
            ast_node_expr_t *expr_node = group_node->expr;
            if (EXPR_NODE_STATIC_TYPE(expr_node) == TYPE_UNKNWN) {
                ast_node_trace_data_type(scope, expr_node);
            }
            group_node->static_type = expr_node->static_type;
            return 0;
        }
        case AST_NODE_TYPE_UNARY_OP: {
            ast_node_unary_op_t *unary_op = (ast_node_unary_op_t *) node;
            ast_node_expr_t *operand = unary_op->operand;
            if (EXPR_NODE_STATIC_TYPE(operand) == TYPE_UNKNWN) {
                ast_node_trace_data_type(scope, operand);
            }
            unary_op->static_type = unary_op->operand->static_type;
            return 0;
        }
        case AST_NODE_TYPE_BINARY_OP: {
            ast_node_binary_op_t *binary_op = (ast_node_binary_op_t *) node;
            return ast_node_trace_binary_op_data_type(scope, binary_op);
        }
        default: compiler_panic("unable to trace static data type of node with type %d", node->type);
    }
}

opcode_t int_operator_to_opcode(int oper) {
    switch (oper) {
        case ADDITION: return OPCODE_IADD;
        case SUBTRACTION: return OPCODE_ISUB;
        case MULTIPLICATION: return OPCODE_IMUL;
        case DIVISION: return OPCODE_IDIV;
        case UNARY_MINUS: return OPCODE_INEG;
            // invert next operations for if instructions
        case EQUAL_TO: return OPCODE_IEQ;
        case NOT_EQUAL_TO: return OPCODE_INEQ;
        case LESS_THAN: return OPCODE_ILT;
        case NOT_GREATER_THAN: return OPCODE_INGT;
        case GREATER_THAN: return OPCODE_IGT;
        case NOT_LESS_THAN: return OPCODE_INLT;
        default: return 0;
    }
}

opcode_t float_operator_to_opcode(int oper) {
    switch (oper) {
        case ADDITION: return OPCODE_FADD;
        case SUBTRACTION: return OPCODE_FSUB;
        case MULTIPLICATION: return OPCODE_FMUL;
        case DIVISION: return OPCODE_FDIV;
        case UNARY_MINUS: return OPCODE_FNEG;

        case EQUAL_TO: return OPCODE_IEQ;
        case NOT_EQUAL_TO: return OPCODE_INEQ;
        default: return 0;
    }
}
