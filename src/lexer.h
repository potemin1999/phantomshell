/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_LEXER_H
#define SHELL_LEXER_H

#include <bits/types/FILE.h>
#include "types.h"
#include "operator.h"

#ifndef __LEXER_DISABLE_TOKEN_TRACING
#define TRACE_TOKEN(x) printf("%s\n",x);
#else
#define TRACE_TOKEN(x)
#endif
struct psh_lexer_state_t {
    bool_t is_interactive: 1;
    bool_t is_inside_func: 1;
    bool_t is_inside_member_func: 1;
    bool_t is_inside_class: 1;
    int32_t is_inside_selection_stat;
    int32_t is_inside_iteration_stat;
};

extern struct psh_lexer_state_t lexer_state;
extern int use_newline_as_flush;

void lexer_set_in(FILE *in);

void lexer_set_out(FILE *out);

void lexer_parse();

int lexer_handle_operator(unsigned oper);

int lexer_handle_keyword_class();

int lexer_handle_keyword_func();

int lexer_handle_keyword_if();

int lexer_handle_keyword_switch();

int lexer_handle_keyword_while();

int lexer_handle_eof();

int lexer_handle_bool(const char *text_value);

int lexer_handle_int(const char *text_value);

int lexer_handle_float(const char *text_value);

int lexer_handle_char(const char *text_value);

int lexer_handle_string(const char *text_value);

int lexer_handle_identifier(const char *identifier_value);

int lexer_unknown_token(const char *value);

#endif //SHELL_LEXER_H
