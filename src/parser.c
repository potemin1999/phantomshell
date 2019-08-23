/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "lib.h"
#include "compiler.h"
#include "parser.h"
#include "operator.h"
#include "lexer.h"
#include "vm/runtime.h"

#define AST_NEW_NODEnt(type_name, var_name, node_type)         \
    size_t size = AST_NODE_##type_name##_SIZE;                 \
    ast_node_##type_name##_t *(var_name);                      \
    (var_name) = (ast_node_##type_name##_t *) malloc(size);    \
    (var_name)->type = __AST_NODE_TYPE_##node_type;            \
    (var_name)->flags = 0;                                     \
    SET_STRING_FUNC(var_name,ast_node_##type_name##_to_string)

#define AST_NEW_NODEn(type_name, var_name) \
    AST_NEW_NODEnt(type_name,var_name,type_name)

#define AST_NEW_NODE(type) \
    AST_NEW_NODEn(type,node)

#ifdef ENABLE_NODE_STRING_REPR
#define TRACE(text) { __log(text); }

#define TRACEf(text, ...) { __log(text,__VA_ARGS__); }
#define TRACE_AND_FREE(text) { char* text_ptr = (text); __log(text_ptr); free(text_ptr); }
#else
#define TRACE(text)
#define TRACEf(text, ...)
#define TRACE_AND_FREE(text)
#endif

#ifdef ENABLE_NODE_STRING_REPR
#define SET_STRING_FUNC(node, func) node->str = func;
#else
#define SET_STRING_FUNC(node, func)
#endif

#define NODE_TYPE(hi_name, lo_name) const unsigned char __AST_NODE_TYPE_##lo_name = AST_NODE_TYPE_##hi_name;
#define NODE_SIZEs(name, size) const int AST_NODE_##name##_SIZE = size;
#define NODE_SIZE(name) NODE_SIZEs(name,sizeof(ast_node_##name##_t));

#ifndef __PARSER_DISABLE_TYPE_CHECK
#define ASSERT_NON_NULL(ptr, text)              \
    if (!(ptr)) {                                 \
    char buffer[256];                           \
    sprintf(buffer,"%s: %s",__FUNCTION__,text); \
    yyerror(text);                              \
    }
#else
#define ASSERT_NON_NULL(ptr, text)
#endif

#ifndef __PARSER_DISABLE_TYPE_CHECK
#define NODE_UPCAST(node, node_out_p, type_id) {        \
    void **node_out_pp = (void**) (node_out_p);           \
    *node_out_pp = ( (node)->type == (type_id) ? (node) : 0 );\
    }
#else
#define NODE_UPCAST(node, node_out, type_id)    \
     void **node_out_p = (void**) (node_out);   \
    *node_out_p = (node);
#endif

#define NODE_UPCAST_GROUP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_GROUP)
#define NODE_UPCAST_SCOPE(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_SCOPE)
#define NODE_UPCAST_IDENT(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_IDENT)
#define NODE_UPCAST_UNARY_OP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_UNARY_OP)
#define NODE_UPCAST_BINARY_OP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_BINARY_OP)
#define NODE_UPCAST_TERNARY_OP(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_TERNARY_OP)
#define NODE_UPCAST_STAT_EXPR(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_STAT_EXPR)
#define NODE_UPCAST_STAT_RET(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_STAT_RET)
#define NODE_UPCAST_STAT_IF(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_STAT_IF)
#define NODE_UPCAST_STAT_SWITCH(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_STAT_SWITCH)
#define NODE_UPCAST_STAT_WHILE(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_STAT_WHILE)
#define NODE_UPCAST_DECL_FUNC(node, node_out) NODE_UPCAST(node,node_out,AST_NODE_TYPE_DECL_FUNC)

#define NODE_UPCAST_EXPR(node_in, node_out){ \
    void **node_out_p = (void**) (node_out); \
    unsigned m = AST_NODE_EXPR_MASK;        \
    unsigned masked = ((node_in)->type & m);  \
    if (masked >> 6u == 1u){                \
        *node_out_p = node_in;              \
    } else {                                \
        *node_out_p = 0;                    \
    }}

void __log(const char *format, ...) {
    printf("->");
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
}

NODE_TYPE(STAT_LIST, stat_list)
NODE_TYPE(STAT_EXPR, stat_expr)
NODE_TYPE(STAT_RET, stat_ret)
NODE_TYPE(STAT_IF, stat_if)
NODE_TYPE(STAT_SWITCH, stat_switch)
NODE_TYPE(STAT_SWITCH_CASE, stat_switch_case)
NODE_TYPE(STAT_SWITCH_OTHER, stat_switch_other)
NODE_TYPE(STAT_WHILE, stat_while)
NODE_TYPE(DECL_VAR, decl_var)
NODE_TYPE(DECL_FUNC, decl_func)
NODE_TYPE(FUNC_ARG_LIST, func_arg_list)
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
NODE_TYPE(SPECIAL_CAST, special_cast)

NODE_SIZE(group)
NODE_SIZE(scope)
NODE_SIZE(ident)
NODE_SIZE(literal)
NODE_SIZE(unary_op)
NODE_SIZE(binary_op)
NODE_SIZE(ternary_op)
NODE_SIZE(decl_var)
NODE_SIZE(decl_func)
NODE_SIZE(func_arg_list)
NODE_SIZE(stat_list)
NODE_SIZE(stat_expr)
NODE_SIZE(stat_ret)
NODE_SIZE(stat_if)
NODE_SIZE(stat_switch)
NODE_SIZE(stat_switch_choice)
NODE_SIZE(stat_while)
NODE_SIZE(special_cast)

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
    return "stringify error: null";
#else
    return "(node str: define ENABLE_NODE_STRING_REPR to parser.h)";
#endif
}

ast_node_t *ast_new_node_group(ast_node_t *expr) {
    ast_node_expr_t *expr_node;
    NODE_UPCAST_EXPR(expr, &expr_node)
    ASSERT_NON_NULL(expr_node, "group expr cast failed")
    AST_NEW_NODE(group)
    node->static_type = 0;
    node->type = AST_NODE_TYPE_GROUP;
    node->expr = expr_node;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_scope(ast_node_t *statements) {
    AST_NEW_NODE(scope)
    node->type = AST_NODE_TYPE_SCOPE;
    //TODO: upcast
    node->stats = (ast_node_stat_list_t *) statements;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_ident(char *value) {
    ASSERT_NON_NULL(value, "ident value cannot be null")
    AST_NEW_NODE(ident)
    node->static_type = TYPE_UNKNWN;
    node->value = value;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_bool(bool_t value) {
    AST_NEW_NODEnt(literal, node, literal_bool)
    node->flags = TYPE_BOOL;
    node->static_type = RUNTIME_TYPE_BOOL;
    node->bool_val = value;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_int(int_t value) {
    AST_NEW_NODEnt(literal, node, literal_int)
    node->flags = TYPE_INT;
    node->static_type = RUNTIME_TYPE_INT;
    node->int_val = value;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_float(float_t value) {
    AST_NEW_NODEnt(literal, node, literal_float)
    node->flags = TYPE_FLOAT;
    node->static_type = RUNTIME_TYPE_FLOAT;
    node->float_val = value;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_char(char_t value) {
    AST_NEW_NODEnt(literal, node, literal_char)
    node->flags = TYPE_CHAR;
    node->static_type = RUNTIME_TYPE_CHAR;
    node->char_val = value;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_literal_string(string_t value) {
    AST_NEW_NODEnt(literal, node, literal_string)
    node->flags = TYPE_STRING;
    node->static_type = RUNTIME_TYPE_STRING;
    node->string_val = value;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_unary_op(uint32_t operator, ast_node_t *operand) {
    ast_node_expr_t *e_operand;
    NODE_UPCAST_EXPR(operand, &e_operand)
    ASSERT_NON_NULL(e_operand, "unary op operand cast fail")
    AST_NEW_NODE(unary_op)
    node->type = AST_NODE_TYPE_UNARY_OP;
    node->static_type = TYPE_UNKNWN;
    node->operator = operator;
    node->operand = e_operand;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_binary_op(uint32_t operator, ast_node_t *left, ast_node_t *right) {
    TRACE("binary op ")
    ast_node_expr_t *e_left;
    NODE_UPCAST_EXPR(left, &e_left)
    ASSERT_NON_NULL(e_left, "binary op left cast fail")
    ast_node_expr_t *e_right;
    NODE_UPCAST_EXPR(right, &e_right)
    ASSERT_NON_NULL(e_right, "binary op right cast fail")
    AST_NEW_NODE(binary_op)
    node->static_type = TYPE_UNKNWN;
    node->operator = operator;
    node->left = e_left;
    node->right = e_right;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_ternary_op(uint32_t operator, ast_node_t *op1, ast_node_t *op2, ast_node_t *op3) {
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
    node->static_type = TYPE_UNKNWN;
    node->operator = operator;
    node->operand1 = e_op1;
    node->operand2 = e_op2;
    node->operand3 = e_op3;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_list(ast_node_t *stat) {
    ast_node_stat_t *stat_node = (ast_node_stat_t *) stat;
    //printf("new stat list node : first %p",stat);
    AST_NEW_NODE(stat_list)
    node->first = stat_node;
    node->last = stat_node;
    if (stat) {
        node->size = 1;
        stat_node->next = 0;
    } else {
        node->size = 0;
    }
    return (ast_node_t *) node;
}

ast_node_t *ast_link_node_stat_list(ast_node_t *node1, ast_node_t *node2) {
    ast_node_stat_list_t *stats = (ast_node_stat_list_t *) node1;
    if (stats->last == 0) {
        //printf("     list first==last==0; type=%u:\n",node2->type);
        stats->first = (ast_node_stat_t *) node2;
        stats->last = (ast_node_stat_t *) node2;
        stats->last->next = 0;
    } else {
        //printf("     chaining %u.next<-%u:\n",stats->last->type,node2->type);
        stats->last->next = (struct ast_node_stat_t *) node2;
        stats->last = (ast_node_stat_t *) node2;
        stats->last->next = 0;
    }
    stats->size++;
    //printf("     %s\n",node2->str(node2));
    return (ast_node_t *) stats;
}

ast_node_t *ast_new_node_stat_expr(ast_node_t *expr) {
    ast_node_expr_t *expr_node;
    NODE_UPCAST_EXPR(expr, &expr_node)
    ASSERT_NON_NULL(expr_node, "stat expr expression cast fail")
    AST_NEW_NODE(stat_expr)
    node->next = 0;
    node->expr = expr_node;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    if (!lexer_state.is_inside_func
        && !lexer_state.is_inside_member_func
        && !lexer_state.is_inside_selection_stat
        && !lexer_state.is_inside_iteration_stat) {
        int res = compiler_compile((ast_node_t *) node);
        UNUSED(res)
        ast_free_node_stat_expr(node);
        node = 0;
    }
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_ret(ast_node_t *expr) {
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat ret expression cast fail")
    AST_NEW_NODE(stat_ret)
    node->next = 0;
    node->expr = expr;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_if(ast_node_t *expr, ast_node_t *true_scope, ast_node_t *false_scope) {
    NODE_UPCAST_EXPR(expr, &expr)
    ASSERT_NON_NULL(expr, "stat if expression cast fail")
    ast_node_scope_t *n_true_scope;
    NODE_UPCAST_SCOPE(true_scope, &n_true_scope)
    ASSERT_NON_NULL(n_true_scope, "stat if true_scope cast fail")
    ast_node_scope_t *n_false_scope = 0;
    if (false_scope) {
        NODE_UPCAST_SCOPE(false_scope, &n_false_scope)
        ASSERT_NON_NULL(n_false_scope, "stat if false_scope cast fail")
    }
    AST_NEW_NODE(stat_if)
    node->next = 0;
    node->expr = expr;
    node->true_scope = n_true_scope;
    node->false_scope = n_false_scope;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    if (!lexer_state.is_inside_func
        && !lexer_state.is_inside_member_func
        && !(lexer_state.is_inside_selection_stat - 1)
        && !lexer_state.is_inside_iteration_stat) {
        int res = compiler_compile((ast_node_t *) node);
        UNUSED(res)
        ast_free_node_stat_if(node);
        node = 0;
    }
    --lexer_state.is_inside_selection_stat;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_switch(ast_node_t *expr, ast_node_t *choice) {
    AST_NEW_NODE(stat_switch)
    node->next = 0;
    node->expr = expr;
    node->choice = choice;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_switch_choice(ast_node_t *expr, ast_node_t *scope, ast_node_t *prev_choice) {
    AST_NEW_NODEnt(stat_switch_choice, node, stat_switch_case)
    if (!expr) {
        node->type = AST_NODE_TYPE_STAT_SWITCH_OTHER;
    }
    node->next = 0;
    node->expr = expr;
    node->scope = scope;
    node->next = 0;
    if (prev_choice) {
        ast_node_stat_switch_choice_t *prev_node = (ast_node_stat_switch_choice_t *) prev_choice;
        prev_node->next = node;
    }
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_stat_while(ast_node_t *expr, ast_node_t *scope) {
    AST_NEW_NODE(stat_while)
    node->next = 0;
    node->expr = expr;
    node->loop_scope = scope;
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    if (!lexer_state.is_inside_func
        && !lexer_state.is_inside_member_func
        && !lexer_state.is_inside_selection_stat
        && !(lexer_state.is_inside_iteration_stat - 1)) {
        int res = compiler_compile((ast_node_t *) node);
        UNUSED(res)
        ast_free_node_stat_while(node);
        node = 0;
    }
    --lexer_state.is_inside_iteration_stat;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_decl_var(string_t name, string_t type) {
    AST_NEW_NODE(decl_var)
    node->var_type = type;
    node->var_name = name;
    node->next = 0;
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_decl_func(string_t name, ast_node_t *args, string_t ret_type, ast_node_t *body) {
    AST_NEW_NODE(decl_func)
    ast_node_scope_t *body_node = 0;
    NODE_UPCAST_SCOPE(body, &body_node)
    node->name = name;
    node->body = body_node;
    node->ret_type = ret_type;
    node->args = 0;
    //TODO: fixxxxxx
    if (args) {
        ast_node_func_arg_list_t *args_node;
        NODE_UPCAST(args, &args_node, AST_NODE_TYPE_FUNC_ARG_LIST)
        ASSERT_NON_NULL(args_node, "cast failed")
        node->args = args_node;
    }
    TRACE_AND_FREE(ast_node_to_string((ast_node_t *) node))
    if (lexer_state.is_inside_class) {
        lexer_state.is_inside_member_func = 0;
    } else {
        lexer_state.is_inside_func = 0;
    }
    if (!lexer_state.is_inside_class) {
        int res = compiler_compile((ast_node_t *) node);
        UNUSED(res)
        ast_free_node_decl_func(node);
        node = 0;
    }
    return (ast_node_t *) node;
}

ast_node_t *ast_new_node_func_arg_list(ast_node_t *decl_var) {
    AST_NEW_NODE(func_arg_list)
    //TODO: upcast
    node->first = (ast_node_decl_var_t *) decl_var;
    node->last = (ast_node_decl_var_t *) decl_var;
    return (ast_node_t *) node;
}

ast_node_special_cast_t *ast_new_node_cast(ast_node_expr_t *expr, static_type_t type, opcode_t opcode) {
    AST_NEW_NODE(special_cast)
    node->static_type = type;
    node->operand = expr;
    node->opcode = opcode;
    return node;
}


static inline void ast_free_node(ast_node_t *node) {
    switch (node->type) {
        case AST_NODE_TYPE_GROUP: {
            ast_free_node_group((ast_node_group_t *) node);
            break;
        }
        case AST_NODE_TYPE_SCOPE: {
            ast_free_node_scope((ast_node_scope_t *) node);
            break;
        }
        case AST_NODE_TYPE_IDENT: {
            ast_free_node_ident((ast_node_ident_t *) node);
            break;
        }
        case AST_NODE_TYPE_LITERAL_BOOL:
        case AST_NODE_TYPE_LITERAL_INT:
        case AST_NODE_TYPE_LITERAL_FLOAT:
        case AST_NODE_TYPE_LITERAL_CHAR:
        case AST_NODE_TYPE_LITERAL_STRING: {
            ast_free_node_literal((ast_node_literal_t *) node);
            break;
        }
        case AST_NODE_TYPE_UNARY_OP: {
            ast_free_node_unary_op((ast_node_unary_op_t *) node);
            break;
        }
        case AST_NODE_TYPE_BINARY_OP: {
            ast_free_node_binary_op((ast_node_binary_op_t *) node);
            break;
        }
        case AST_NODE_TYPE_TERNARY_OP: {
            ast_free_node_ternary_op((ast_node_ternary_op_t *) node);
            break;
        }
        case AST_NODE_TYPE_SPECIAL_CAST: {
            ast_free_node_cast((ast_node_special_cast_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_LIST: {
            ast_free_node_stat_list((ast_node_stat_list_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_EXPR: {
            ast_free_node_stat_expr((ast_node_stat_expr_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_RET: {
            ast_free_node_stat_ret((ast_node_stat_ret_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_IF: {
            ast_free_node_stat_if((ast_node_stat_if_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_SWITCH: {
            ast_free_node_stat_switch((ast_node_stat_switch_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_SWITCH_OTHER:
        case AST_NODE_TYPE_STAT_SWITCH_CASE: {
            ast_free_node_stat_switch_choice((ast_node_stat_switch_choice_t *) node);
            break;
        }
        case AST_NODE_TYPE_STAT_WHILE: {
            ast_free_node_stat_while((ast_node_stat_while_t *) node);
            break;
        }
        case AST_NODE_TYPE_DECL_FUNC : {
            ast_free_node_decl_func((ast_node_decl_func_t *) node);
            break;
        }
        default: {
            compiler_panic("unexpected node type to free = %x", node->type);
        }
    }
}

void ast_free_node_group(ast_node_group_t *group) {
    ast_free_node((ast_node_t *) group->expr);
    free(group);
}

void ast_free_node_scope(ast_node_scope_t *scope) {
    ast_free_node_stat_list(scope->stats);
    free(scope);
}

void ast_free_node_ident(ast_node_ident_t *ident) {
    free(ident->value);
    free(ident);
}

void ast_free_node_literal(ast_node_literal_t *literal) {
    if (literal->type == AST_NODE_TYPE_LITERAL_STRING) {
        free(literal->string_val);
    }
    free(literal);
}

void ast_free_node_unary_op(ast_node_unary_op_t *unary_op) {
    ast_free_node((ast_node_t *) unary_op->operand);
    free(unary_op);
}

void ast_free_node_binary_op(ast_node_binary_op_t *binary_op) {
    ast_free_node((ast_node_t *) binary_op->left);
    ast_free_node((ast_node_t *) binary_op->right);
    free(binary_op);
}

void ast_free_node_ternary_op(ast_node_ternary_op_t *ternary_op) {
    ast_free_node((ast_node_t *) ternary_op->operand1);
    ast_free_node((ast_node_t *) ternary_op->operand2);
    ast_free_node((ast_node_t *) ternary_op->operand3);
    free(ternary_op);
}

void ast_free_node_cast(ast_node_special_cast_t *cast) {
    ast_free_node((ast_node_t *) cast->operand);
    free(cast);
}

void ast_free_node_stat_list(ast_node_stat_list_t *stat_list) {
    ast_node_stat_t *stat = stat_list->first;
    while (stat) {
        struct ast_node_stat_t *next = stat->next;
        ast_free_node((ast_node_t *) stat);
        stat = (ast_node_stat_t *) next;
    }
    free(stat_list);
}

void ast_free_node_stat_expr(ast_node_stat_expr_t *stat_expr) {
    ast_free_node((ast_node_t *) stat_expr->expr);
    free(stat_expr);
}

void ast_free_node_stat_ret(ast_node_stat_ret_t *stat_ret) {
    ast_free_node((ast_node_t *) stat_ret->expr);
    free(stat_ret);
}

void ast_free_node_stat_if(ast_node_stat_if_t *stat_if) {
    ast_free_node((ast_node_t *) stat_if->expr);
    ast_free_node_scope(stat_if->true_scope);
    if (stat_if->false_scope) {
        ast_free_node_scope(stat_if->false_scope);
    }
    free(stat_if);
}

void ast_free_node_stat_switch(ast_node_stat_switch_t *stat_switch) {
    ast_free_node((ast_node_t *) stat_switch->expr);
    ast_node_stat_switch_choice_t *choice = (ast_node_stat_switch_choice_t *) stat_switch->choice;
    while (choice) {
        ast_node_stat_switch_choice_t *next = choice->next;
        ast_free_node_stat_switch_choice(choice);
        choice = next;
    }
    free(stat_switch);
}

void ast_free_node_stat_switch_choice(ast_node_stat_switch_choice_t *stat_choice) {
    if (stat_choice->expr) {
        ast_free_node((ast_node_t *) stat_choice->expr);
    }
    ast_free_node(stat_choice->scope);
    free(stat_choice);
}

void ast_free_node_stat_while(ast_node_stat_while_t *stat_while) {
    if (stat_while->expr) {
        ast_free_node((ast_node_t *) stat_while->expr);
    }
    ast_free_node((ast_node_t *) stat_while->loop_scope);
    free(stat_while);
}

void ast_free_node_decl_var(ast_node_decl_var_t *var_decl) {
    free(var_decl->var_name);
    free(var_decl->var_type);
    free(var_decl);
}

void ast_free_node_decl_func(ast_node_decl_func_t *decl_func) {
    ast_free_node((ast_node_t *) decl_func->body);
    free(decl_func->name);
    ast_free_node_func_arg_list(decl_func->args);
    if (decl_func->ret_type) {
        free(decl_func->ret_type);
    }
    free(decl_func);
}

void ast_free_node_func_arg_list(ast_node_func_arg_list_t *func_arg_list) {
    ast_node_decl_var_t *current_decl = func_arg_list->first;
    while (current_decl) {
        ast_node_decl_var_t *next = current_decl->next;
        ast_free_node_decl_var(current_decl);
        current_decl = next;
    }
    free(func_arg_list);
}


#ifdef ENABLE_NODE_STRING_REPR

NODE_TO_STRING_FUNC(stub) {
    char *buffer = (char *) malloc(32);
    sprintf(buffer, "(not impl str node = %d)", node->type);
    return buffer;
}

NODE_TO_STRING_FUNC(group) {
    ast_node_group_t *group_node;
    NODE_UPCAST_GROUP(node, &group_node)
    ASSERT_NON_NULL(group_node, "cast failed")
    string_t expr_str = ast_node_to_string((ast_node_t *) group_node->expr);
    size_t expr_len = strlen(expr_str);
    char *buffer = (char *) malloc(expr_len + 8);
    sprintf(buffer, "( %s )", expr_str);
    free(expr_str);
    return buffer;
}

NODE_TO_STRING_FUNC(ident) {
    ast_node_ident_t *ident_node;
    NODE_UPCAST_IDENT(node, &ident_node)
    ASSERT_NON_NULL(ident_node, "cast failed")
    char *buffer = (char *) malloc(128);
    sprintf(buffer, "(ident=%s)", ident_node->value);
    return buffer;
}

NODE_TO_STRING_FUNC(unary_op) {
    ast_node_unary_op_t *op_node;
    NODE_UPCAST_UNARY_OP(node, &op_node)
    ASSERT_NON_NULL(op_node, "cast failed")
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
    NODE_UPCAST_BINARY_OP(node, &op_node)
    ASSERT_NON_NULL(op_node, "cast failed")
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
    NODE_UPCAST_TERNARY_OP(node, &op_node)
    ASSERT_NON_NULL(op_node, "cast failed")
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
    NODE_UPCAST_SCOPE(node, &scope_node)
    ASSERT_NON_NULL(scope_node, "cast failed")
    if (scope_node->stats) {
        ast_node_t *stats = (ast_node_t *) scope_node->stats;
        string_t stats_str = ast_node_to_string(stats);
        size_t stats_len = strlen(stats_str);
        char *buffer = (char *) malloc(stats_len + 8);
        sprintf(buffer, "{%s}", stats_str);
        free(stats_str);
        return buffer;
    } else {
        return strdup("{empty}");
    }
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
        default: {
            compiler_panic("unexpected literal type");
        }
    }
}

NODE_TO_STRING_FUNC(stat_list) {
    return strdup("statements");
}

NODE_TO_STRING_FUNC(stat_expr) {
    ast_node_stat_expr_t *stat_node;
    NODE_UPCAST_STAT_EXPR(node, &stat_node)
    ASSERT_NON_NULL(stat_node, "cast failed")
    string_t expr_str = ast_node_to_string((ast_node_t *) stat_node->expr);
    size_t expr_len = strlen(expr_str);
    char *buffer = (char *) malloc(expr_len + 8);
    sprintf(buffer, "(%s ;)", expr_str);
    free(expr_str);
    return buffer;
}

NODE_TO_STRING_FUNC(stat_ret) {
    ast_node_stat_ret_t *stat_node;
    NODE_UPCAST_STAT_RET(node, &stat_node)
    ASSERT_NON_NULL(stat_node, "cast failed")
    string_t expr_str = ast_node_to_string(stat_node->expr);
    size_t expr_len = strlen(expr_str);
    char *buffer = (char *) malloc(expr_len + 8);
    sprintf(buffer, "(ret %s ;)", expr_str);
    free(expr_str);
    return buffer;
}

NODE_TO_STRING_FUNC(stat_if) {
    ast_node_stat_if_t *stat_node;
    NODE_UPCAST_STAT_IF(node, &stat_node)
    ASSERT_NON_NULL(stat_node, "cast failed")
    string_t expr_str = ast_node_to_string(stat_node->expr);
    string_t true_str = ast_node_to_string((ast_node_t *) stat_node->true_scope);
    string_t false_str = 0;
    if (stat_node->false_scope) {
        false_str = ast_node_to_string((ast_node_t *) stat_node->false_scope);
    } else {
        false_str = strdup("{none}");
    }
    size_t expr_len = strlen(expr_str);
    size_t true_len = strlen(true_str);
    size_t false_len = strlen(false_str);
    char *buffer = (char *) malloc(expr_len + true_len + false_len + 32);
    sprintf(buffer, "(if %s  \n  then %s\n  else %s)", expr_str, true_str, false_str);
    free(expr_str);
    free(true_str);
    free(false_str);
    return buffer;
}

NODE_TO_STRING_FUNC(stat_switch) {
    ast_node_stat_switch_t *switch_node;
    NODE_UPCAST_STAT_SWITCH(node, &switch_node)
    ASSERT_NON_NULL(switch_node, "cast failed")
    string_t expr_str = ast_node_to_string(switch_node->expr);
    size_t expr_len = strlen(expr_str);

    ast_node_stat_switch_choice_t *cur_choice;
    size_t choices_len = 0;
    cur_choice = (ast_node_stat_switch_choice_t *) switch_node->choice;
    for (; cur_choice; cur_choice = cur_choice->next, choices_len++);
    char *choices_str[choices_len];
    size_t choices_str_len[choices_len];
    size_t choices_sum_len = 0;
    cur_choice = (ast_node_stat_switch_choice_t *) switch_node->choice;
    for (int i = 0; cur_choice; cur_choice = cur_choice->next, i++) {
        choices_str[i] = ast_node_to_string((ast_node_t *) cur_choice);
        choices_str_len[i] = strlen(choices_str[i]);
        choices_sum_len += choices_str_len[i];
    }
    char *choices_buffer = (char *) malloc(choices_sum_len + choices_len + 1);
    for (size_t i = 0, ptr = 0; i < choices_len; i++) {
        strcpy(choices_buffer + ptr, choices_str[i]);
        free(choices_str[i]);
        ptr += choices_str_len[i];
        strcpy(choices_buffer + ptr, "\n");
        ptr += 1;
    }
    choices_buffer[choices_sum_len + choices_len] = '\0';
    size_t choices_buffer_len = choices_sum_len + choices_len;

    char *buffer = (char *) malloc(choices_buffer_len + expr_len + 16);
    sprintf(buffer, "( %s switch to %s)", expr_str, choices_buffer);
    free(expr_str);
    free(choices_buffer);
    return buffer;
}

NODE_TO_STRING_FUNC(stat_switch_choice) {
    ast_node_stat_switch_choice_t *choice_node = 0;
    if (node->type == AST_NODE_TYPE_STAT_SWITCH_CASE ||
        node->type == AST_NODE_TYPE_STAT_SWITCH_OTHER) {
        choice_node = (ast_node_stat_switch_choice_t *) node;
    }
    ASSERT_NON_NULL(choice_node, "cast failed")
    ast_node_t *expr = choice_node->expr;
    string_t expr_str = expr ? ast_node_to_string(expr) : strdup("other");
    size_t expr_len = strlen(expr_str);
    string_t scope_str = ast_node_to_string(choice_node->scope);
    size_t scope_len = strlen(scope_str);

    char *buffer = (char *) malloc(expr_len + scope_len + 16);
    sprintf(buffer, "case %s -> %s", expr_str, scope_str);
    free(expr_str);
    free(scope_str);
    return buffer;
}

NODE_TO_STRING_FUNC(stat_while) {
    ast_node_stat_while_t *stat_node;
    NODE_UPCAST_STAT_WHILE(node, &stat_node)
    string_t expr_str = ast_node_to_string(stat_node->expr);
    size_t expr_len = strlen(expr_str);
    string_t body_str = ast_node_to_string(stat_node->loop_scope);
    size_t body_len = strlen(body_str);
    char *buffer = (char *) malloc(body_len + expr_len + 16);
    sprintf(buffer, "(while %s do %s)", expr_str, body_str);
    free(expr_str);
    free(body_str);
    return buffer;
}

NODE_TO_STRING_FUNC(decl_var) {
    //TODO: implement
    ast_node_decl_var_t *decl_node;
    NODE_UPCAST(node, &decl_node, AST_NODE_TYPE_DECL_VAR)
    ASSERT_NON_NULL(decl_node, "cast failed")
    string_t name = decl_node->var_name;
    string_t type = decl_node->var_type;
    size_t name_size = strlen(name);
    size_t type_size = strlen(type);
    char *buffer = (char *) malloc(name_size + type_size + 16);
    sprintf(buffer, "%s of type %s", name, type);
    return buffer;
}

NODE_TO_STRING_FUNC(decl_func) {
    ast_node_decl_func_t *func_node;
    NODE_UPCAST_DECL_FUNC(node, &func_node)
    ASSERT_NON_NULL(func_node, "cast failed")
    size_t func_arg_count = 0;
    if (func_node->args) {
        ast_node_func_arg_list_t *arg = func_node->args;
        ast_node_decl_var_t *current = arg->first;
        while (current) {
            func_arg_count++;
            current = current->next;
        }
    }
    string_t func_args_str = 0;
    if (func_arg_count != 0) {
        char *args[func_arg_count];
        ast_node_func_arg_list_t *args_list = func_node->args;
        ast_node_decl_var_t *arg = args_list->first;
        for (size_t i = 0; arg; i++, arg = arg->next) {
            args[i] = ast_node_to_string((ast_node_t *) arg);
        }
        size_t args_str_len = 0;
        size_t args_len[func_arg_count];
        for (size_t i = 0; i < func_arg_count; i++) {
            args_len[i] = strlen(args[i]);
            args_str_len += args_len[i];
        }
        func_args_str = (char *) malloc(args_str_len + func_arg_count + 1);
        size_t current_ptr = 0;
        for (size_t i = 0; i < func_arg_count; i++) {
            strcpy(func_args_str + current_ptr, args[i]);
            free(args[i]);
            strcpy(func_args_str + current_ptr + args_len[i], ",");
            current_ptr += (args_len[i] + 1);
        }
        func_args_str[args_str_len + func_arg_count] = '\0';
    } else {
        func_args_str = strdup("");
    }
    string_t body_str = ast_node_to_string((ast_node_t *) func_node->body);
    size_t body_len = strlen(body_str);
    size_t name_len = strlen(func_node->name);
    string_t ret_str = func_node->ret_type ? func_node->ret_type : "";
    size_t ret_len = strlen(ret_str);
    size_t args_len = strlen(func_args_str);
    char *buffer = (char *) malloc(name_len + args_len + ret_len + body_len + 16);
    sprintf(buffer, "(func %s (%s) %s %s)", func_node->name, func_args_str, func_node->ret_type, body_str);
    free(body_str);
    free(func_args_str);
    return buffer;
}

NODE_TO_STRING_FUNC(func_arg_list) {
    return strdup("delegate to decl func");
}

NODE_TO_STRING_FUNC(special_cast) {
    char *buffer = (char *) malloc(16);
    sprintf(buffer, "(cast %s)", "operand");
    return buffer;
}

#endif

void yyerror(const char *str) {
    fprintf(stdout, "Parsing error: %s\n", str);
}

int yywrap() {
    return 1;
}

