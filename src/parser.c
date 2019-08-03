/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "parser.h"
#include "operator.h"

#define AST_ALLOCATE(p_ptr, size) ( *p_ptr = (void*) malloc(size) )

#define TRACE(text) { __log(text); }

#define TRACEf(text, ...) { __log(text,__VA_ARGS__); }

#define EXPECT_NODE(type, node)                          \
    if (node->type != type) {                           \
        char buffer[128];                               \
        char *str = "expected node type %d, got %d\n";  \
        sprintf(buffer,,type,node->type);               \
        yyerror(buffer);                                \
    }

#ifdef ENABLE_NODE_STRING_REPR
#define SET_STRING_FUNC(node, func) node->str = func;
#else
#define SET_STRING_FUNC(node,func)
#endif

#define NODE_TO_STRING_FUNC(type) string_t ast_node_##type##_to_string(ast_node_t *node)
#define NODE_SIZE(name, struct_t) const int name = sizeof(struct_t);


#define ASSERT_NON_NULL(ptr, text) \
    if (!ptr) { \
    char buffer[256];\
    sprintf(buffer,"%s: %s",__FUNCTION__,text);\
    yyerror(text);\
    }

#define NODE_UPCAST(node, node_out_p, type_id) {   \
    void **node_out_pp = (void**) node_out_p;           \
    *node_out_pp = ( node->type == type_id ? node : 0 );\
    }

#define NODE_UPCAST_GROUP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_GROUP)

#define NODE_UPCAST_SCOPE(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_SCOPE)

#define NODE_UPCAST_IDENT(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_IDENT)

#define NODE_UPCAST_LITERAL(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_LITERAL)

#define NODE_UPCAST_UNARY_OP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_UNARY_OP)

#define NODE_UPCAST_BINARY_OP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_BINARY_OP)

#define NODE_UPCAST_TERNARY_OP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_TERNARY_OP)

#define NODE_UPCAST_EXPR(node_in, node_out){ \
    void **node_out_p = (void**) node_out;  \
    unsigned m = AST_NODE_EXPR_MASK;        \
    unsigned masked = (node_in->type & m);  \
    if (masked >> 6u == 1u){                \
        *node_out_p = node_in;              \
    } else {                                \
        *node_out_p = 0;                    \
    }}

#define NODE_UPCAST_STAT(node_in, node_out)  \
    void **node_out_p = (void**) node_out;  \
    switch (node_in->type) {                \
        case AST_NODE_TYPE_STAT_EXPR:       \
        case AST_NODE_TYPE_STAT_IF:         \
        case AST_NODE_TYPE_STAT_RET:        \
        case AST_NODE_TYPE_STAT_WHILE:      \
        case AST_NODE_TYPE_STAT_SWITCH:     \
            *node_out_p = node_in;          \
            break;                          \
        default:                            \
            *node_out_p = 0;                \
    }

void __log(const char *format, ...) {
    printf("->");
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
}


string_t ast_node_to_string(ast_node_t *node) {
    #ifdef ENABLE_NODE_STRING_REPR
    if (node->str) {
        return node->str(node);
    }
    #else
    return "node stringify mode disabled, add ENABLE_NODE_STRING_REPR to parser.h";
    #endif
}

NODE_TO_STRING_FUNC(group);

NODE_TO_STRING_FUNC(ident);

NODE_TO_STRING_FUNC(unary_op);

NODE_TO_STRING_FUNC(binary_op);

NODE_TO_STRING_FUNC(ternary_op);


NODE_SIZE(NODE_GROUP_SIZE, ast_node_group_t)
NODE_SIZE(NODE_SCOPE_SIZE, ast_node_scope_t)
NODE_SIZE(NODE_IDENT_SIZE, ast_node_ident_t)
NODE_SIZE(NODE_LITERAL_SIZE, ast_node_literal_t)
NODE_SIZE(NODE_UNARY_OP_SIZE, ast_node_unary_op_t)
NODE_SIZE(NODE_BINARY_OP_SIZE, ast_node_binary_op_t)
NODE_SIZE(NODE_TERNARY_OP_SIZE, ast_node_ternary_op_t)
NODE_SIZE(NODE_DECL_FUNC_SIZE, ast_node_decl_func_t)
NODE_SIZE(NODE_FUNC_ARG_SIZE, ast_node_func_arg_t)
NODE_SIZE(NODE_STAT_EXPR_SIZE, ast_node_stat_expr_t)
NODE_SIZE(NODE_STAT_IF_SIZE, ast_node_stat_if_t)
NODE_SIZE(NODE_STAT_SWITCH_SIZE, ast_node_stat_switch_t)
NODE_SIZE(NODE_STAT_SWITCH_CHOICE_SIZE, ast_node_stat_switch_choice_t)
NODE_SIZE(NODE_STAT_WHILE_SIZE, ast_node_stat_while_t)


ast_node_t *ast_new_node_group(ast_node_t *expr) {
    TRACEf("group of %d", expr->type)
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "group expr cast failed")
    ast_node_group_t *node;
    AST_ALLOCATE(&node, NODE_GROUP_SIZE);
    SET_STRING_FUNC(node, ast_node_group_to_string)
    node->type = AST_NODE_TYPE_GROUP;
    node->expr = expr;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_scope(ast_node_t *statements) {
    TRACEf("scope of %d", statements->type);
    ast_node_scope_t *node;
    AST_ALLOCATE(&node, NODE_SCOPE_SIZE);
    node->type = AST_NODE_TYPE_SCOPE;
    node->stats = statements;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_ident(const char *value) {
    ASSERT_NON_NULL(value, "ident value cannot be null")
    ast_node_ident_t *node;
    AST_ALLOCATE(&node, NODE_IDENT_SIZE);
    SET_STRING_FUNC(node, ast_node_ident_to_string)
    node->type = AST_NODE_TYPE_IDENT;
    node->value = value;
    TRACEf("%s", node->str((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_bool(bool_t value) {
    TRACE("literal bool")
    ast_node_literal_t *node;
    AST_ALLOCATE(&node, NODE_LITERAL_SIZE);
    node->type = AST_NODE_TYPE_LITERAL_BOOL;
    node->bool_val = value;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_int(int_t value) {
    TRACE("literal int")
    ast_node_literal_t *node;
    AST_ALLOCATE(&node, NODE_LITERAL_SIZE);
    node->type = AST_NODE_TYPE_LITERAL_INT;
    node->int_val = value;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_float(float_t value) {
    TRACE("literal float")
    ast_node_literal_t *node;
    AST_ALLOCATE(&node, NODE_LITERAL_SIZE);
    node->type = AST_NODE_TYPE_LITERAL_FLOAT;
    node->float_val = value;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_char(char_t value) {
    TRACE("literal char")
    ast_node_literal_t *node;
    AST_ALLOCATE(&node, NODE_LITERAL_SIZE);
    node->type = AST_NODE_TYPE_LITERAL_CHAR;
    node->char_val = value;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_string(string_t value) {
    TRACE("literal string")
    ast_node_literal_t *node;
    AST_ALLOCATE(&node, NODE_LITERAL_SIZE);
    node->type = AST_NODE_TYPE_LITERAL_STRING;
    node->string_val = value;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_unary_op(int operator, ast_node_t *operand) {
    ast_node_expr_t *e_operand;
    NODE_UPCAST_EXPR(operand, &e_operand)
    ASSERT_NON_NULL(e_operand, "unary op operand cast fail")
    ast_node_unary_op_t *node;
    AST_ALLOCATE(&node, NODE_UNARY_OP_SIZE);
    SET_STRING_FUNC(node, ast_node_unary_op_to_string)
    node->type = AST_NODE_TYPE_UNARY_OP;
    node->operator = operator;
    node->operand = e_operand;
    TRACEf("%s", node->str((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_binary_op(int operator, ast_node_t *left, ast_node_t *right) {
    TRACE("binary op ")
    ast_node_expr_t *e_left;
    NODE_UPCAST_EXPR(left, &e_left)
    ASSERT_NON_NULL(e_left, "binary op left cast fail")
    ast_node_expr_t *e_right;
    NODE_UPCAST_EXPR(right, &e_right)
    ASSERT_NON_NULL(e_right, "binary op right cast fail")
    ast_node_binary_op_t *node;
    AST_ALLOCATE(&node, NODE_BINARY_OP_SIZE);
    node->type = AST_NODE_TYPE_BINARY_OP;
    node->operator = operator;
    node->left = e_left;
    node->right = e_right;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_ternary_op(int operator, ast_node_t *op1, ast_node_t *op2, ast_node_t *op3) {
    TRACE("ternary op")
    ast_node_expr_t *e_op1;
    NODE_UPCAST_EXPR(op1, &e_op1)
    ASSERT_NON_NULL(e_op1, "ternary op operand 1 cast fail")
    ast_node_expr_t *e_op2;
    NODE_UPCAST_EXPR(op2, &e_op2)
    ASSERT_NON_NULL(e_op2, "ternary op operand 2 cast fail")
    ast_node_expr_t *e_op3;
    NODE_UPCAST_EXPR(op3, &e_op3)
    ASSERT_NON_NULL(e_op3, "ternary op operand 3 cast fail")
    ast_node_ternary_op_t *node;
    AST_ALLOCATE(&node, NODE_TERNARY_OP_SIZE);
    node->type = AST_NODE_TYPE_TERNARY_OP;
    node->operator = operator;
    node->operand1 = e_op1;
    node->operand2 = e_op2;
    node->operand3 = e_op3;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_expr(ast_node_t *expr) {
    TRACE("stat expr")
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat expr expression cast fail")
    ast_node_stat_expr_t *node;
    AST_ALLOCATE(&node, NODE_STAT_EXPR_SIZE);
    node->type = AST_NODE_TYPE_STAT_EXPR;
    node->expr = expr;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_ret(ast_node_t *expr) {
    TRACE("stat ret")
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat ret expression cast fail")
    ast_node_stat_ret_t *node;
    AST_ALLOCATE(&node, AST_NODE_TYPE_STAT_RET);
    node->type = AST_NODE_TYPE_STAT_RET;
    node->expr = expr;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_if(ast_node_t *expr, ast_node_t *true_scope, ast_node_t *false_scope) {
    TRACE("stat if")
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat if expression cast fail")
    ast_node_scope_t *n_true_scope;
    NODE_UPCAST_EXPR(true_scope, &n_true_scope)
    ASSERT_NON_NULL(n_true_scope, "stat if true_scope cast fail")
    ast_node_scope_t *n_false_scope;
    NODE_UPCAST_EXPR(false_scope, &n_false_scope)
    ASSERT_NON_NULL(n_false_scope, "stat if false_scope cast fail")
    ast_node_stat_if_t *node;
    AST_ALLOCATE(&node, NODE_STAT_IF_SIZE);
    node->type = AST_NODE_TYPE_STAT_IF;
    node->expr = expr;
    node->true_scope = n_true_scope;
    node->false_scope = n_false_scope;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_switch(ast_node_t *expr, ast_node_t *choice) {
    TRACE("stat switch")
    ast_node_stat_switch_t *node;
    AST_ALLOCATE(&node, NODE_STAT_SWITCH_SIZE);
    node->type = AST_NODE_TYPE_STAT_SWITCH;
    node->expr = expr;
    node->choice = choice;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_switch_choice(ast_node_t *expr, ast_node_t *scope, ast_node_t *prev_choice) {
    TRACE("stat switch choice")
    ast_node_stat_switch_choice_t *node;
    AST_ALLOCATE(&node, NODE_STAT_SWITCH_CHOICE_SIZE);
    node->type = (expr ? AST_NODE_TYPE_STAT_SWITCH_CASE : AST_NODE_TYPE_STAT_SWITCH_OTHER);
    node->expr = expr;
    node->scope = scope;
    node->next = 0;
    //TODO: chain previous
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_while(ast_node_t *expr, ast_node_t *scope) {
    TRACE("stat while")
    ast_node_stat_while_t *node;
    AST_ALLOCATE(&node, NODE_STAT_WHILE_SIZE);
    node->type = AST_NODE_TYPE_STAT_WHILE;
    node->expr = expr;
    node->loop_scope = scope;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_decl_func(string_t name, ast_node_t *args, string_t ret_type, ast_node_t *body) {
    TRACE("func decl")
    ast_node_decl_func_t *node;
    AST_ALLOCATE(&node, NODE_DECL_FUNC_SIZE);
    node->type = AST_NODE_TYPE_DECL_FUNC;
    node->name = name;
    node->args = args;
    node->body = body;
    node->ret_type = ret_type;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_func_arg(string_t type, string_t name, ast_node_t *prev) {
    TRACE("func arg")
    ast_node_func_arg_t *node;
    AST_ALLOCATE(&node, NODE_FUNC_ARG_SIZE);
    node->type = AST_NODE_TYPE_FUNC_ARG;
    node->arg_name = name;
    node->arg_type = type;
    node->next = 0;
    return (ast_node_t *) node;
}

#ifdef ENABLE_NODE_STRING_REPR

NODE_TO_STRING_FUNC(stub) {
    char *buffer = (char *) malloc(32);
    sprintf(buffer, "(not impl str node = %d)", node->type);
    return buffer;
}

NODE_TO_STRING_FUNC(group) {
    return 0;
}

NODE_TO_STRING_FUNC(ident) {
    ast_node_ident_t *ident_node;
    NODE_UPCAST_IDENT(node, &ident_node);
    ASSERT_NON_NULL(ident_node, "cast failed");
    char *buffer = (char *) malloc(128);
    sprintf(buffer, "(ident=%s)", ident_node->value);
    return buffer;
}

NODE_TO_STRING_FUNC(unary_op) {
    ast_node_unary_op_t *op_node;
    NODE_UPCAST_UNARY_OP(node, &op_node);
    ASSERT_NON_NULL(op_node, "cast failed");
    ast_node_t *operand = (ast_node_t *) op_node->operand;
    string_t operand_str = operand->str(operand);
    size_t operand_len = strlen(operand_str);
    char *buffer = (char *) malloc(operand_len + 16);
    sprintf(buffer, "(unary %d %s)", op_node->operator, operand_str);
    free(operand_str);
    return buffer;
}

NODE_TO_STRING_FUNC(binary_op) {
    ast_node_binary_op_t *op_node;
    NODE_UPCAST_BINARY_OP(node, &op_node);
    ASSERT_NON_NULL(op_node, "cast failed");
    ast_node_t *left = (ast_node_t *) op_node->left;
    return 0;
}

NODE_TO_STRING_FUNC(ternary_op) {
    return 0;
}

#endif

void yyerror(const char *str) {
    fprintf(stderr, "Parsing error: %s\n", str);
}

int yywrap() {
    return 1;
}

