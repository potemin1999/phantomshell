/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */


#include "lexer.h"
#include <string.h>
#include <stdlib.h>
#include "generated/y.tab.h"
#include "parser.h"
#include "compiler.h"

int use_newline_as_flush = 1;

struct psh_lexer_state_t lexer_state = {
        .is_interactive = 1,
        .is_inside_func = 0,
        .is_inside_member_func = 0,
        .is_inside_class = 0
};

int lexer_handle_eof() {
    compiler_finish();
    return 0;
}

int lexer_handle_keyword_class() {
    TRACE_TOKEN("CLASS")
    lexer_state.is_inside_class = 1;
    return CLASS;
}

int lexer_handle_keyword_func() {
    TRACE_TOKEN("FUNC")
    if (lexer_state.is_inside_class) {
        if (lexer_state.is_inside_member_func) {
            yyerror("enclosed member functions are not permitted");
        }
        lexer_state.is_inside_member_func = 1;
    } else {
        if (lexer_state.is_inside_func) {
            yyerror("enclosed functions are not permitted");
        }
        lexer_state.is_inside_func = 1;
    }
    return FUNC;
}

int lexer_handle_keyword_if() {
    TRACE_TOKEN("IF")
    lexer_state.is_inside_selection_stat++;
    return IF;
}

int lexer_handle_keyword_switch() {
    TRACE_TOKEN("SWITCH")
    lexer_state.is_inside_selection_stat++;
    return SWITCH;
}

int lexer_handle_keyword_while() {
    TRACE_TOKEN("WHILE")
    lexer_state.is_inside_iteration_stat++;
    return WHILE;
}

int lexer_handle_bool(const char *text_value) {
    TRACE_TOKEN("BooleanLiteral")
    if (strcmp(text_value, "true") == 0) {
        yylval.bool_value = 1;
    } else {
        yylval.bool_value = 0;
    }
    return BooleanLiteral;
}

int lexer_handle_int(const char *text_value) {
    TRACE_TOKEN("IntegerLiteral")
    size_t len = strlen(text_value);
    char start[len], *end;
    strcpy(start, text_value);
    int value = (int) strtol(text_value, &end, 10);
    yylval.int_value = value;
    return IntegerLiteral;
}

int lexer_handle_float(const char *text_value) {
    TRACE_TOKEN("FloatLiteral")
    size_t len = strlen(text_value);
    char start[len], *end;
    strcpy(start, text_value);
    yylval.float_value = (float) strtod(start, &end);
    return FloatLiteral;
}

int lexer_handle_char(const char *text_value) {
    TRACE_TOKEN("CharLiteral")
    yylval.char_value = *(text_value + 1);
    return CharLiteral;
}

int lexer_handle_string(const char *text_value) {
    TRACE_TOKEN("StringLiteral")
    size_t len = strlen(text_value);
    char *str_dup = (char *) malloc(len - 1);
    memcpy(str_dup, text_value + 1, len - 2);
    str_dup[len - 2] = '\0';
    yylval.string_value = str_dup;
    return StringLiteral;
}

int lexer_handle_identifier(const char *identifier_value) {
    TRACE_TOKEN("Identifier")
    yylval.string_value = strdup(identifier_value);
    return Identifier;
}

int lexer_unknown_token(const char *value) {
    TRACE_TOKEN("Unexpected Token")
    printf("token %s is unexpected\n", value);
    return 0;
}

void yyset_in(FILE *_in_str);

void yyset_out(FILE *_out_str);

int yyget_debug(void);

void yyset_debug(int _bdebug);

void lexer_set_in(FILE *in) {
    yyset_in(in);
}

void lexer_set_out(FILE *out) {
    yyset_out(out);
}

void lexer_parse() {
    yyparse();
}