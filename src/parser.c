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

#define AST_NEW_NODEnt(type_name, var_name, node_type)              \
    size_t size = AST_NODE_##type_name##_SIZE;               \
    ast_node_##type_name##_t *var_name;                          \
    var_name = (ast_node_##type_name##_t *) malloc(size);    \
    var_name->type = __AST_NODE_TYPE_##node_type;            \
    SET_STRING_FUNC(var_name,ast_node_##type_name##_to_string)

#define AST_NEW_NODEn(type_name, var_name) \
    AST_NEW_NODEnt(type_name,var_name,type_name)

#define AST_NEW_NODE(type) \
    AST_NEW_NODEn(type,node)

#define TRACE(text) { \
    __log(text);      \
    }

#define TRACEf(text, ...) {  \
    __log(text,__VA_ARGS__); \
    }

#ifdef ENABLE_NODE_STRING_REPR
#define SET_STRING_FUNC(node, func) node->str = func;
#else
#define SET_STRING_FUNC(node,func)
#endif

#define NODE_TYPE(hi_name, lo_name) const unsigned char __AST_NODE_TYPE_##lo_name = AST_NODE_TYPE_##hi_name;
#define NODE_SIZEs(name, size) const int AST_NODE_##name##_SIZE = size;
#define NODE_SIZE(name) NODE_SIZEs(name,sizeof(ast_node_##name##_t));

#define ASSERT_NON_NULL(ptr, text)              \
    if (!ptr) {                                 \
    char buffer[256];                           \
    sprintf(buffer,"%s: %s",__FUNCTION__,text); \
    yyerror(text);                              \
    }

#define NODE_UPCAST(node, node_out_p, type_id) {        \
    void **node_out_pp = (void**) node_out_p;           \
    *node_out_pp = ( node->type == type_id ? node : 0 );\
    }

#define NODE_UPCAST_GROUP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_GROUP)
#define NODE_UPCAST_SCOPE(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_SCOPE)
#define NODE_UPCAST_IDENT(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_IDENT)
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

NODE_TYPE(STAT_EXPR, stat_expr)
NODE_TYPE(STAT_RET, stat_ret)
NODE_TYPE(STAT_IF, stat_if)
NODE_TYPE(STAT_SWITCH, stat_switch)
NODE_TYPE(STAT_SWITCH_CASE, stat_switch_case)
NODE_TYPE(STAT_SWITCH_OTHER, stat_switch_other)
NODE_TYPE(STAT_WHILE, stat_while)
NODE_TYPE(DECL_FUNC, decl_func)
NODE_TYPE(FUNC_ARG, func_arg)
NODE_TYPE(GROUP, group)
NODE_TYPE(SCOPE, scope)
NODE_TYPE(IDENT, ident)
NODE_TYPE(LITERAL_BOOL, literal_bool)
NODE_TYPE(LITERAL_INT, literal_int)
NODE_TYPE(LITERAL_FLOAT, literal_float)
NODE_TYPE(LITERAL_CHAR, literal_char)
NODE_TYPE(LITERAL_STRING, literal_string)
NODE_TYPE(UNARY_OP, unary_op)
NODE_TYPE(BINARY_OP, binary_op)
NODE_TYPE(TERNARY_OP, ternary_op)

NODE_SIZE(group)
NODE_SIZE(scope)
NODE_SIZE(ident)
NODE_SIZE(literal)
NODE_SIZE(unary_op)
NODE_SIZE(binary_op)
NODE_SIZE(ternary_op)
NODE_SIZE(decl_func)
NODE_SIZE(func_arg)
NODE_SIZE(stat_expr)
NODE_SIZE(stat_ret)
NODE_SIZE(stat_if)
NODE_SIZE(stat_switch)
NODE_SIZE(stat_switch_choice)
NODE_SIZE(stat_while)

NODE_SIZEs(literal_bool, sizeof(ast_node_literal_t))
NODE_SIZEs(literal_int, sizeof(ast_node_literal_t))
NODE_SIZEs(literal_float, sizeof(ast_node_literal_t))
NODE_SIZEs(literal_char, sizeof(ast_node_literal_t))
NODE_SIZEs(literal_string, sizeof(ast_node_literal_t))

string_t ast_node_to_string(ast_node_t *node) {
    #ifdef ENABLE_NODE_STRING_REPR
    if (node->str) {
        return node->str(node);
    }
    #else
    return "node stringify mode disabled, add ENABLE_NODE_STRING_REPR to parser.h";
    #endif
}

ast_node_t *ast_new_node_group(ast_node_t *expr) {
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "group expr cast failed")
    AST_NEW_NODE(group)
    node->type = AST_NODE_TYPE_GROUP;
    node->expr = expr;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_scope(ast_node_t *statements) {
    AST_NEW_NODE(scope)
    node->type = AST_NODE_TYPE_SCOPE;
    node->stats = statements;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_ident(const char *value) {
    ASSERT_NON_NULL(value, "ident value cannot be null")
    AST_NEW_NODE(ident)
    node->type = AST_NODE_TYPE_IDENT;
    node->value = value;
    TRACEf("%s", node->str((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_bool(bool_t value) {
    AST_NEW_NODEnt(literal, node, literal_bool)
    node->type = AST_NODE_TYPE_LITERAL_BOOL;
    node->bool_val = value;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_int(int_t value) {
    AST_NEW_NODEnt(literal, node, literal_int)
    node->int_val = value;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_float(float_t value) {
    AST_NEW_NODEnt(literal, node, literal_float)
    node->float_val = value;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_char(char_t value) {
    AST_NEW_NODEnt(literal, node, literal_char)
    node->char_val = value;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_string(string_t value) {
    AST_NEW_NODEnt(literal, node, literal_string)
    node->string_val = value;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_unary_op(int operator, ast_node_t *operand) {
    ast_node_expr_t *e_operand;
    NODE_UPCAST_EXPR(operand, &e_operand)
    ASSERT_NON_NULL(e_operand, "unary op operand cast fail")
    AST_NEW_NODE(unary_op)
    node->type = AST_NODE_TYPE_UNARY_OP;
    node->operator = operator;
    node->operand = e_operand;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
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
    AST_NEW_NODE(binary_op)
    node->operator = operator;
    node->left = e_left;
    node->right = e_right;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_ternary_op(int operator, ast_node_t *op1, ast_node_t *op2, ast_node_t *op3) {
    ast_node_expr_t *e_op1;
    NODE_UPCAST_EXPR(op1, &e_op1)
    ASSERT_NON_NULL(e_op1, "ternary op operand 1 cast fail")
    ast_node_expr_t *e_op2;
    NODE_UPCAST_EXPR(op2, &e_op2)
    ASSERT_NON_NULL(e_op2, "ternary op operand 2 cast fail")
    ast_node_expr_t *e_op3;
    NODE_UPCAST_EXPR(op3, &e_op3)
    ASSERT_NON_NULL(e_op3, "ternary op operand 3 cast fail")
    AST_NEW_NODE(ternary_op)
    node->operator = operator;
    node->operand1 = e_op1;
    node->operand2 = e_op2;
    node->operand3 = e_op3;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_expr(ast_node_t *expr) {
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat expr expression cast fail")
    AST_NEW_NODE(stat_expr)
    node->expr = expr;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_ret(ast_node_t *expr) {
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat ret expression cast fail")
    AST_NEW_NODE(stat_ret)
    node->expr = expr;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_if(ast_node_t *expr, ast_node_t *true_scope, ast_node_t *false_scope) {
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat if expression cast fail")
    ast_node_scope_t *n_true_scope;
    NODE_UPCAST_EXPR(true_scope, &n_true_scope)
    ASSERT_NON_NULL(n_true_scope, "stat if true_scope cast fail")
    ast_node_scope_t *n_false_scope;
    NODE_UPCAST_EXPR(false_scope, &n_false_scope)
    ASSERT_NON_NULL(n_false_scope, "stat if false_scope cast fail")
    AST_NEW_NODE(stat_if)
    node->expr = expr;
    node->true_scope = n_true_scope;
    node->false_scope = n_false_scope;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_switch(ast_node_t *expr, ast_node_t *choice) {
    AST_NEW_NODE(stat_switch)
    node->expr = expr;
    node->choice = choice;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_switch_choice(ast_node_t *expr, ast_node_t *scope, ast_node_t *prev_choice) {
    //AST NEW NODE
    AST_NEW_NODEnt(stat_switch_choice, node, stat_switch_case)
    if (!expr) {
        node->type = AST_NODE_TYPE_STAT_SWITCH_OTHER;
    }
    node->expr = expr;
    node->scope = scope;
    node->next = 0;
    //TODO: chain previous
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_while(ast_node_t *expr, ast_node_t *scope) {
    AST_NEW_NODE(stat_while)
    node->expr = expr;
    node->loop_scope = scope;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_decl_func(string_t name, ast_node_t *args, string_t ret_type, ast_node_t *body) {
    AST_NEW_NODE(decl_func)
    node->name = name;
    node->args = args;
    node->body = body;
    node->ret_type = ret_type;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_func_arg(string_t type, string_t name, ast_node_t *prev) {
    AST_NEW_NODE(func_arg)
    node->arg_name = name;
    node->arg_type = type;
    node->next = 0;
    TRACEf("%s", ast_node_to_string((ast_node_t *) node))
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
    string_t operand_str = ast_node_to_string(operand);
    size_t operand_len = strlen(operand_str);
    char *buffer = (char *) malloc(operand_len + 16);
    sprintf(buffer, "(%d %s)", op_node->operator, operand_str);
    free(operand_str);
    return buffer;
}

NODE_TO_STRING_FUNC(binary_op) {
    ast_node_binary_op_t *op_node;
    NODE_UPCAST_BINARY_OP(node, &op_node);
    ASSERT_NON_NULL(op_node, "cast failed");
    ast_node_t *left = (ast_node_t *) op_node->left;
    ast_node_t *right = (ast_node_t *) op_node->right;
    string_t left_str = ast_node_to_string(left);
    string_t right_str = ast_node_to_string(right);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);
    char *buffer = (char *) malloc(left_len + right_len + 16);
    sprintf(buffer, "(%s %d %s)", left_str, op_node->operator, right_str);
    free(left_str);
    free(right_str);
    return buffer;
}

NODE_TO_STRING_FUNC(ternary_op) {
    ast_node_ternary_op_t *op_node;
    NODE_UPCAST_TERNARY_OP(node, &op_node);
    ASSERT_NON_NULL(op_node, "cast failed");
    ast_node_t *op1 = (ast_node_t *) op_node->operand1;
    ast_node_t *op2 = (ast_node_t *) op_node->operand2;
    ast_node_t *op3 = (ast_node_t *) op_node->operand3;
    string_t op1_str = ast_node_to_string(op1);
    string_t op2_str = ast_node_to_string(op2);
    string_t op3_str = ast_node_to_string(op3);
    size_t op1_len = strlen(op1_str);
    size_t op2_len = strlen(op2_str);
    size_t op3_len = strlen(op3_str);
    char *buffer = (char *) malloc(op1_len + op2_len + op3_len + 16);
    sprintf(buffer, "(%s %d %s %s)", op1_str, op_node->operator, op2_str, op3_str);
    free(op1);
    free(op2);
    free(op3);
    return buffer;
}

NODE_TO_STRING_FUNC(scope) {
    ast_node_scope_t *scope_node;
    NODE_UPCAST_SCOPE(node, &scope_node);
    ASSERT_NON_NULL(scope_node, "cast failed");
    ast_node_t *stats = scope_node->stats;
    string_t stats_str = ast_node_to_string(stats);
    size_t stats_len = strlen(stats_str);
    char *buffer = (char *) malloc(stats_len + 8);
    sprintf(buffer, "({%s})", stats_str);
    free(stats_str);
    return buffer;
}

NODE_TO_STRING_FUNC(literal) {
    ast_node_literal_t *literal_node;
    literal_node = (ast_node_literal_t *) node;
    string_t str_repr;
    switch (node->type) {
        case AST_NODE_TYPE_LITERAL_BOOL: {
            str_repr = (string_t) malloc(16);
            sprintf(str_repr, "L(B)=%s", literal_node->bool_val ? "true" : "false");
            return str_repr;
        }
        case AST_NODE_TYPE_LITERAL_INT: {
            str_repr = (string_t) malloc(24);
            sprintf(str_repr, "L(I)=%d", literal_node->int_val);
            return str_repr;
        }
        case AST_NODE_TYPE_LITERAL_FLOAT: {
            str_repr = (string_t) malloc(32);
            sprintf(str_repr, "L(F)=%f", literal_node->float_val);
            return str_repr;
        }
        case AST_NODE_TYPE_LITERAL_CHAR: {
            str_repr = (string_t) malloc(16);
            sprintf(str_repr, "L(C)=%c", literal_node->char_val);
            return str_repr;
        }
        case AST_NODE_TYPE_LITERAL_STRING: {
            string_t val = literal_node->string_val;
            str_repr = (string_t) malloc(8 + strlen(val));
            sprintf(str_repr, "L(S)=%s", val);
            return str_repr;
        }
    }
    return strdup("(error: unexpected literal type)");
}

NODE_TO_STRING_FUNC(stat_expr) {
    return strdup("stat_expr");
}

NODE_TO_STRING_FUNC(stat_ret) {
    return strdup("stat_ret");
}

NODE_TO_STRING_FUNC(stat_if) {
    return strdup("stat_if");
}

NODE_TO_STRING_FUNC(stat_switch) {
    return strdup("stat_switch");
}

NODE_TO_STRING_FUNC(stat_switch_choice) {
    return strdup("stat_switch");
}

NODE_TO_STRING_FUNC(stat_while) {
    return strdup("stat_while");
}

NODE_TO_STRING_FUNC(decl_func) {
    return strdup("decl_func");
}

NODE_TO_STRING_FUNC(func_arg) {
    return strdup("func_args");
}

#endif

void yyerror(const char *str) {
    fprintf(stderr, "Parsing error: %s\n", str);
}

int yywrap() {
    return 1;
}

