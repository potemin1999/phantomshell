/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/17/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "parser.h"

using namespace phlib;
using namespace psh;

Parser::Parser() {
    tokenBuffer = (Token *) phlib::malloc(PARSER_TOKEN_BUFFER_CAPACITY * sizeof(Token *));
    pushFunctions[TokenType::IDENTIFIER] = &Parser::pushIdentifierToken;
    pushFunctions[TokenType::KEYWORD]    = &Parser::pushKeywordToken;
    pushFunctions[TokenType::LITERAL]    = &Parser::pushLiteralToken;
    pushFunctions[TokenType::OPERATOR]   = &Parser::pushOperatorToken;
    pushFunctions[TokenType::SEPARATOR]  = &Parser::pushSeparatorToken;
}

Parser::~Parser() {
    phlib::free(tokenBuffer);
}

int Parser::pushToken(Token *token) {
    TokenType &type = token->type;
    if (type < 1 | type > 5)
        return -1;
    return (this->*(pushFunctions[type]))(token);
}

int Parser::pushIdentifierToken(Token *token) {
    return 0;
}

int Parser::pushKeywordToken(Token *token) {
    return 0;
}

int Parser::pushLiteralToken(Token *token) {
    return 0;
}

int Parser::pushOperatorToken(Token *token) {
    return 0;
}

int Parser::pushSeparatorToken(Token *token) {
    return 0;
}
