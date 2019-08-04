/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "compiler.h"
#include "opcodes.h"

struct cast_request_t select_cast_method(ubyte_t type_id1, ubyte_t type_id2) {

}

int ast_node_trace_data_type(ast_node_expr_t *node) {
    if (EXPR_NODE_TYPE(node) != TYPE_UNKNWN) {
        return 0;
    }
    switch (node->type) {
        case AST_NODE_TYPE_UNARY_OP: {
            ast_node_unary_op_t *unary_op = (ast_node_unary_op_t *) node;
            ast_node_expr_t *operand = unary_op->operand;
            if (EXPR_NODE_TYPE(operand) == TYPE_UNKNWN) {
                ast_node_trace_data_type(operand);
            }
            unary_op->flags &= 0b11110000u;
            unary_op->flags |= (unary_op->operand->flags & 0b00001111u);
            return 0;
        }
        case AST_NODE_TYPE_BINARY_OP: {
            ast_node_binary_op_t *binary_op = (ast_node_binary_op_t *) node;
            ast_node_expr_t *left = binary_op->left;
            ast_node_expr_t *right = binary_op->right;
            if (EXPR_NODE_TYPE(left) == TYPE_UNKNWN) {
                ast_node_trace_data_type(left);
            }
            if (EXPR_NODE_TYPE(right) == TYPE_UNKNWN) {
                ast_node_trace_data_type(right);
            }
            ubyte_t left_type = EXPR_NODE_TYPE(left);
            ubyte_t right_type = EXPR_NODE_TYPE(right);
            ubyte_t result_type = 0;
            if (left_type != right_type) {
                printf("%s %d warn : cast not implemented", __FILE__, __LINE__);
                result_type = left_type;
            } else {
                result_type = right_type;
            }
            binary_op->flags &= 0b11110000u;
            binary_op->flags |= result_type;
            return 0;
        }
    }
}

opcode_t int_operator_to_opcode(int oper) {
    switch (oper) {
        case ADDITION: return OPCODE_IADD;
        case SUBTRACTION: return OPCODE_ISUB;
        case MULTIPLICATION: return OPCODE_IMUL;
        case DIVISION: return OPCODE_IDIV;
        default: return 0;
    }
}
