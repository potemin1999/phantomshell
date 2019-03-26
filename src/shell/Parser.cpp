/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/17/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Parser.h"

using namespace phlib;
using namespace psh;

typedef int (*PushTokenFunc)(const Token &);

class Parser::ParserData {
public:
    Token *tokenBuffer;             /**< Stack of tokens*/
    PushTokenFunc pushFunctions[6]; /**< Array of token push functions for each token type */
};

int PushIdentifierToken(const Token &token);

int PushKeywordToken(const Token &token);

int PushLiteralToken(const Token &token);

int PushOperatorToken(const Token &token);

int PushSeparatorToken(const Token &token);

Parser::Parser() {
    this->data = new ParserData();
    auto tokenBufferSize = PARSER_TOKEN_BUFFER_CAPACITY * sizeof(Token);
    this->data->tokenBuffer = static_cast<Token *>(malloc(tokenBufferSize));
    auto pushFunctions = this->data->pushFunctions;
    pushFunctions[TokenType::IDENTIFIER] = &PushIdentifierToken;
    pushFunctions[TokenType::KEYWORD] = &PushKeywordToken;
    pushFunctions[TokenType::LITERAL] = &PushLiteralToken;
    pushFunctions[TokenType::OPERATOR] = &PushOperatorToken;
    pushFunctions[TokenType::SEPARATOR] = &PushSeparatorToken;
}

Parser::~Parser() {
    free(data->tokenBuffer);
    delete data;
}

int Parser::pushToken(Token *token) {
    TokenType &type = token->type;
    if (type < 1 | type > 5)
        return -1;
    return (*this->data->pushFunctions[type])(*token);
}

int PushIdentifierToken(const Token &token) {
    return 0;
}

int PushKeywordToken(const Token &token) {
    return 0;
}

int PushLiteralToken(const Token &token) {
    return 0;
}

int PushOperatorToken(const Token &token) {
    return 0;
}

int PushSeparatorToken(const Token &token) {
    return 0;
}
