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
#include "y.tab.h"
#include "parser.h"

int use_newline_as_flush = 0;

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
    int value = atoi(text_value);
    yylval.int_value = value;
    return IntegerLiteral;
}

int lexer_handle_float(const char *text_value) {
    TRACE_TOKEN("FloatLiteral")
    yylval.float_value = atof(text_value);
    return FloatLiteral;
}

int lexer_handle_char(const char *text_value) {
    TRACE_TOKEN("CharLiteral")
    yylval.char_value = *(text_value + 1);
    return CharLiteral;
}

int lexer_handle_string(const char *text_value) {
    TRACE_TOKEN("StringLiteral")
    //TODO: remove ''
    yylval.string_value = strdup(text_value);
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
