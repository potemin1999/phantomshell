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

#include "lib.h"
#include "vm/opcodes.h"
#include "types.h"
#include "generated/y.tab.h"

#define AST_NODE_TYPE_EMPTY 0b00000000
#define AST_NODE_STAT_MASK              0b10000000u //128
#define AST_NODE_TYPE_STAT_EXPR         0b10000001u //129
#define AST_NODE_TYPE_STAT_RET          0b10000010u //130
#define AST_NODE_TYPE_STAT_IF           0b10000011u //131
#define AST_NODE_TYPE_STAT_SWITCH       0b10000100u //132
#define AST_NODE_TYPE_STAT_SWITCH_CASE  0b10000101u //133
#define AST_NODE_TYPE_STAT_SWITCH_OTHER 0b10000110u //134
#define AST_NODE_TYPE_STAT_WHILE        0b10000111u //137
#define AST_NODE_TYPE_STAT_LIST         0b10001111u //145
#define AST_NODE_TYPE_DECL_VAR          0b00000010u //2
#define AST_NODE_TYPE_DECL_FUNC         0b00000100u //4
#define AST_NODE_TYPE_FUNC_ARG_LIST     0b00000101u //5
#define AST_NODE_EXPR_MASK              0b01000000u //64
#define AST_NODE_TYPE_SCOPE             0b00000011u //3
#define AST_NODE_TYPE_IDENT             0b01000001u //65
#define AST_NODE_TYPE_GROUP             0b01000010u //66
#define AST_NODE_TYPE_LITERAL_BOOL      0b01000011u //67
#define AST_NODE_TYPE_LITERAL_INT       0b01000100u //68
#define AST_NODE_TYPE_LITERAL_FLOAT     0b01000101u //69
#define AST_NODE_TYPE_LITERAL_CHAR      0b01000110u //70
#define AST_NODE_TYPE_LITERAL_STRING    0b01000111u //71
#define AST_NODE_TYPE_UNARY_OP          0b01001000u //72
#define AST_NODE_TYPE_BINARY_OP         0b01001001u //73
#define AST_NODE_TYPE_TERNARY_OP        0b01001010u //74
#define AST_NODE_TYPE_SPECIAL_CAST      0b01001011u //75

#define NODE_TO_STRING_FUNC(type) string_t ast_node_##type##_to_string(ast_node_t *node)

#ifndef __PARSER_DISABLE_STRINGIFY
#define ENABLE_NODE_STRING_REPR 1
#endif

typedef string_t (*to_string_func)(struct ast_node_t *);

#ifdef ENABLE_NODE_STRING_REPR
#define AST_NODE_HEADER()   \
    unsigned char type;     \
    unsigned char flags;    \
    to_string_func str;
#else
#define AST_NODE_HEADER()   \
    unsigned char type;     \
    unsigned char flags;
#endif

#define AST_EXPR_HEADER()           \
    static_type_t static_type;      \
    struct ast_node_expr_t *next;

#define AST_STAT_HEADER()     \
    struct ast_node_stat_t *next;


#define DEF_AST_NODE(name, body) \
    typedef                     \
    struct ast_node_##name##_t{ \
        AST_NODE_HEADER()       \
        struct body;            \
    }ast_node_##name##_t;       \
    NODE_TO_STRING_FUNC(name);

#define DEF_AST_NODEe(name, body) \
    typedef struct {            \
        AST_NODE_HEADER()       \
        AST_EXPR_HEADER()       \
        struct body;            \
    }ast_node_##name##_t;       \
    NODE_TO_STRING_FUNC(name);

#define DEF_AST_NODEs(name, body) \
    typedef struct {            \
        AST_NODE_HEADER()       \
        AST_STAT_HEADER()       \
        struct body;            \
    }ast_node_##name##_t;       \
    NODE_TO_STRING_FUNC(name);

typedef struct ast_node_t {
    AST_NODE_HEADER()
} ast_node_t;

/**
 * Every structure define below represent an Abstract Syntax Tree node
 * Header contains enough information to understand the size of structure and its values
 * IF DEFINED ENABLE_NODE_STRING_REPR
 *   Each node struct should implement ast_node_##type_name##_to_string function, as it autowired while allocation
 * ENDIF
 */

DEF_AST_NODEe(expr, {})

DEF_AST_NODEs(stat, {})

DEF_AST_NODEs(stat_list, {
    int32_t size;
    ast_node_stat_t *first;
    ast_node_stat_t *last;
})

DEF_AST_NODEe(group, {
    ast_node_expr_t *expr;
})

DEF_AST_NODE(scope, {
    ast_node_stat_list_t *stats;
})

DEF_AST_NODEe(ident, {
    char *value;
})

DEF_AST_NODEe(literal, {
    union {
        bool_t bool_val;
        int_t int_val;
        float_t float_val;
        char_t char_val;
        string_t string_val;
    };
})

DEF_AST_NODEe(unary_op, {
    uint32_t operator;
    ast_node_expr_t *operand;
})

DEF_AST_NODEe(binary_op, {
    uint32_t operator;
    ast_node_expr_t *left;
    ast_node_expr_t *right;
})

DEF_AST_NODEe(ternary_op, {
    uint32_t operator;
    ast_node_expr_t *operand1;
    ast_node_expr_t *operand2;
    ast_node_expr_t *operand3;
})

DEF_AST_NODEs(stat_expr, {
    ast_node_expr_t *expr;
})

DEF_AST_NODEs(stat_ret, {
    ast_node_t *expr;
})

DEF_AST_NODEs(stat_if, {
    ast_node_t *expr;
    ast_node_scope_t *true_scope;
    ast_node_scope_t *false_scope;
})

DEF_AST_NODEs(stat_switch, {
    ast_node_t *expr;
    ast_node_t *choice;
})

DEF_AST_NODE(stat_switch_choice, {
    ast_node_t *expr;
    ast_node_t *scope;
    struct ast_node_stat_switch_choice_t *next;
})

DEF_AST_NODEs(stat_while, {
    ast_node_t *expr;
    ast_node_t *loop_scope;
})

DEF_AST_NODE(decl_var, {
    string_t var_type;
    string_t var_name;
    // used in function declaration
    struct ast_node_decl_var_t *next;
})

DEF_AST_NODE(func_arg_list, {
    ast_node_decl_var_t *first;
    ast_node_decl_var_t *last;
})

DEF_AST_NODE(decl_func, {
    string_t name;
    string_t ret_type;
    ast_node_func_arg_list_t *args;
    ast_node_scope_t *body;
})

DEF_AST_NODEe(special_cast, {
    opcode_t opcode;
    ast_node_expr_t *operand;
})

NODE_TO_STRING_FUNC(literal_bool);

NODE_TO_STRING_FUNC(literal_int);

NODE_TO_STRING_FUNC(literal_float);

NODE_TO_STRING_FUNC(literal_char);

NODE_TO_STRING_FUNC(literal_string);

string_t ast_node_to_string(ast_node_t *node);

ast_node_t *ast_new_node_group(ast_node_t *expr);

void ast_free_node_group(ast_node_group_t *group);

ast_node_t *ast_new_node_scope(ast_node_t *statements);

void ast_free_node_scope(ast_node_scope_t *scope);

// Expression nodes

ast_node_t *ast_new_node_ident(char *value);

void ast_free_node_ident(ast_node_ident_t *ident);

ast_node_t *ast_new_node_literal_bool(bool_t value);

ast_node_t *ast_new_node_literal_int(int_t value);

ast_node_t *ast_new_node_literal_float(float_t value);

ast_node_t *ast_new_node_literal_char(char_t value);

ast_node_t *ast_new_node_literal_string(string_t value);

void ast_free_node_literal(ast_node_literal_t *literal);

ast_node_t *ast_new_node_unary_op(uint32_t operator, ast_node_t *operand);

void ast_free_node_unary_op(ast_node_unary_op_t *unary_op);

ast_node_t *ast_new_node_binary_op(uint32_t operator, ast_node_t *left, ast_node_t *right);

void ast_free_node_binary_op(ast_node_binary_op_t *binary_op);

ast_node_t *ast_new_node_ternary_op(uint32_t operator, ast_node_t *op1, ast_node_t *op2, ast_node_t *op3);

void ast_free_node_ternary_op(ast_node_ternary_op_t *ternary_op);

ast_node_special_cast_t *ast_new_node_cast(ast_node_expr_t *expr, static_type_t type, opcode_t opcode);

void ast_free_node_cast(ast_node_special_cast_t *cast);

// Statement nodes

ast_node_t *ast_new_node_stat_list(ast_node_t *expr);

ast_node_t *ast_link_node_stat_list(ast_node_t *node1, ast_node_t *node2);

void ast_free_node_stat_list(ast_node_stat_list_t *stat_list);

ast_node_t *ast_new_node_stat_expr(ast_node_t *expr);

void ast_free_node_stat_expr(ast_node_stat_expr_t *stat_expr);

ast_node_t *ast_new_node_stat_ret(ast_node_t *expr);

void ast_free_node_stat_ret(ast_node_stat_ret_t *stat_ret);

ast_node_t *ast_new_node_stat_if(ast_node_t *expr, ast_node_t *true_scope, ast_node_t *false_scope);

void ast_free_node_stat_if(ast_node_stat_if_t *stat_if);

ast_node_t *ast_new_node_stat_switch(ast_node_t *expr, ast_node_t *choice);

void ast_free_node_stat_switch(ast_node_stat_switch_t *stat_switch);

ast_node_t *ast_new_node_stat_switch_choice(ast_node_t *expr, ast_node_t *scope, ast_node_t *prev_choice);

void ast_free_node_stat_switch_choice(ast_node_stat_switch_choice_t *stat_choice);

ast_node_t *ast_new_node_stat_while(ast_node_t *expr, ast_node_t *scope);

void ast_free_node_stat_while(ast_node_stat_while_t *stat_while);

// Declaration nodes

ast_node_t *ast_new_node_decl_var(string_t name, string_t type);

void ast_free_node_decl_var(ast_node_decl_var_t *var_decl);

ast_node_t *ast_new_node_decl_func(string_t name, ast_node_t *args, string_t ret_type, ast_node_t *body);

void ast_free_node_decl_func(ast_node_decl_func_t *decl_func);

ast_node_t *ast_new_node_func_arg_list(ast_node_t *decl_var);

void ast_free_node_func_arg_list(ast_node_func_arg_list_t *func_arg_list);

void yyerror(const char *str);

int yywrap();

int yylex(void);

extern int yydebug;

#endif //SHELL_PARSER_H
