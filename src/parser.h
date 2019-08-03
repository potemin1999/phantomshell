/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_PARSER_H
#define SHELL_PARSER_H

#include <stdio.h>
#include "types.h"
#include "y.tab.h"

#define AST_NODE_TYPE_EMPTY 0b00000000
#define AST_NODE_TYPE_STAT_EXPR         0b10000001u
#define AST_NODE_TYPE_STAT_RET          0b10000010u
#define AST_NODE_TYPE_STAT_IF           0b10000011u
#define AST_NODE_TYPE_STAT_SWITCH       0b10000100u
#define AST_NODE_TYPE_STAT_SWITCH_CASE  0b10000101u
#define AST_NODE_TYPE_STAT_SWITCH_OTHER 0b10000110u
#define AST_NODE_TYPE_STAT_WHILE        0b10000111u

#define AST_NODE_TYPE_DECL_FUNC         0b00000100u
#define AST_NODE_TYPE_FUNC_ARG          0b00000101u

#define AST_NODE_EXPR_MASK              0b01000000u
#define AST_NODE_TYPE_GROUP             0b00000010u
#define AST_NODE_TYPE_SCOPE             0b00000011u
#define AST_NODE_TYPE_IDENT             0b01000001u
#define AST_NODE_TYPE_LITERAL_BOOL      0b01000011u
#define AST_NODE_TYPE_LITERAL_INT       0b01000100u
#define AST_NODE_TYPE_LITERAL_FLOAT     0b01000101u
#define AST_NODE_TYPE_LITERAL_CHAR      0b01000110u
#define AST_NODE_TYPE_LITERAL_STRING    0b01000111u
#define AST_NODE_TYPE_UNARY_OP          0b01001000u
#define AST_NODE_TYPE_BINARY_OP         0b01001001u
#define AST_NODE_TYPE_TERNARY_OP        0b01001010u

#define ENABLE_NODE_STRING_REPR 1

typedef string_t (*to_string_func)(struct ast_node_t *);

#ifdef ENABLE_NODE_STRING_REPR
#define AST_NODE_HEADER()   \
    unsigned char type;     \
    to_string_func str;
#else
#define AST_NODE_HEADER()   \
    unsigned char type;
#endif

#define DEF_AST_NODE(name, body) \
    typedef                     \
    struct ast_node_##name##_t{ \
        AST_NODE_HEADER()       \
        struct body;            \
    }ast_node_##name##_t;

typedef struct ast_node_t {
    AST_NODE_HEADER()
} ast_node_t;

DEF_AST_NODE(expr, {})

DEF_AST_NODE(stat, {})

DEF_AST_NODE(group, {
    ast_node_t *expr;
})

DEF_AST_NODE(scope, {
    ast_node_t *stats;
})

DEF_AST_NODE(ident, {
    const char *value;
})

DEF_AST_NODE(literal, {
    union {
        bool_t bool_val;
        int_t int_val;
        float_t float_val;
        char_t char_val;
        string_t string_val;
    };
})

DEF_AST_NODE(unary_op, {
    int operator;
    ast_node_expr_t *operand;
})

DEF_AST_NODE(binary_op, {
    int operator;
    ast_node_expr_t *left;
    ast_node_expr_t *right;
})

DEF_AST_NODE(ternary_op, {
    int operator;
    ast_node_expr_t *operand1;
    ast_node_expr_t *operand2;
    ast_node_expr_t *operand3;
})

DEF_AST_NODE(stat_expr, {
    ast_node_t *expr;
})

DEF_AST_NODE(stat_ret, {
    ast_node_t *expr;
})

DEF_AST_NODE(stat_if, {
    ast_node_t *expr;
    ast_node_scope_t *true_scope;
    ast_node_scope_t *false_scope;
})

DEF_AST_NODE(stat_switch, {
    ast_node_t *expr;
    ast_node_t *choice;
})

DEF_AST_NODE(stat_switch_choice, {
    ast_node_t *expr;
    ast_node_t *scope;
    ast_node_t *next;
})

DEF_AST_NODE(stat_while, {
    ast_node_t *expr;
    ast_node_t *loop_scope;
})

DEF_AST_NODE(decl_func, {
    string_t name;
    string_t ret_type;
    ast_node_t *args;
    ast_node_t *body;
})

DEF_AST_NODE(func_arg, {
    string_t arg_type;
    string_t arg_name;
    ast_node_t *next;
})

string_t ast_node_to_string(ast_node_t *node);

ast_node_t *ast_new_node_group(ast_node_t *expr);

ast_node_t *ast_new_node_scope(ast_node_t *statements);

ast_node_t *ast_new_node_ident(const char *value);

ast_node_t *ast_new_node_literal_bool(bool_t value);

ast_node_t *ast_new_node_literal_int(int_t value);

ast_node_t *ast_new_node_literal_float(float_t value);

ast_node_t *ast_new_node_literal_char(char_t value);

ast_node_t *ast_new_node_literal_string(string_t value);

ast_node_t *ast_new_node_unary_op(int operator, ast_node_t *operand);

ast_node_t *ast_new_node_binary_op(int operator, ast_node_t *left, ast_node_t *right);

ast_node_t *ast_new_node_ternary_op(int operator, ast_node_t *op1, ast_node_t *op2, ast_node_t *op3);

ast_node_t *ast_new_node_stat_expr(ast_node_t *expr);

ast_node_t *ast_new_node_stat_ret(ast_node_t *expr);

ast_node_t *ast_new_node_stat_if(ast_node_t *expr, ast_node_t *true_scope, ast_node_t *false_scope);

ast_node_t *ast_new_node_stat_switch(ast_node_t *expr, ast_node_t *choice);

ast_node_t *ast_new_node_stat_switch_choice(ast_node_t *expr, ast_node_t *scope, ast_node_t *prev_choice);

ast_node_t *ast_new_node_stat_while(ast_node_t *expr, ast_node_t *scope);

ast_node_t *ast_new_node_decl_func(string_t name, ast_node_t *args, string_t ret_type, ast_node_t *body);

ast_node_t *ast_new_node_func_arg(string_t type, string_t name, ast_node_t *prev);


void yyerror(const char *str);

int yywrap();

int yylex(void);

extern int yydebug;

#endif //SHELL_PARSER_H
