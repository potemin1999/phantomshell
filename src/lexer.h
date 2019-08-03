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

#include "types.h"
#include "operator.h"

#define TRACE_TOKEN(x) printf("%s\n",x);

extern int use_newline_as_flush;

int lexer_handle_operator(unsigned oper);

int lexer_handle_bool(const char *text_value);

int lexer_handle_int(const char *text_value);

int lexer_handle_float(const char *text_value);

int lexer_handle_char(const char *text_value);

int lexer_handle_string(const char *text_value);

int lexer_handle_identifier(const char *identifier_value);

int lexer_unknown_token(const char *value);

#endif //SHELL_LEXER_H
