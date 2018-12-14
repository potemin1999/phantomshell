/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/17/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <parser.h>

#include "parser.h"

using namespace psh;

Parser::Parser() {
    token_buffer = (Token*) phlib::malloc(PARSER_TOKEN_BUFFER_CAPACITY*sizeof(Token));
    push_functions[TokenType::IDENTIFIER] = &Parser::push_identifier_token;
    push_functions[TokenType::KEYWORD] = &Parser::push_keyword_token;
    push_functions[TokenType::LITERAL] = &Parser::push_literal_token;
    push_functions[TokenType::OPERATOR] = &Parser::push_operator_token;
    push_functions[TokenType::SEPARATOR] = &Parser::push_separator_token;
}

Parser::~Parser() {
    phlib::free(token_buffer);
}

int Parser::push_token(Token *token) {
    TokenType& type = token->type;
    if (type<1 | type>5)
        return -1;
    return (this->*(push_functions[type]))(token);
}

int Parser::push_identifier_token(Token *token) {
    return 0;
}

int Parser::push_keyword_token(Token *token) {
    return 0;
}

int Parser::push_literal_token(Token *token) {
    return 0;
}

int Parser::push_operator_token(Token *token) {
    return 0;
}

int Parser::push_separator_token(Token *token) {
    return 0;
}
