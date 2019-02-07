/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/14/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PARSER_H
#define PHANTOMSHELL_PARSER_H

#include "keyword.h"
#include "token.h"
#include "tree.h"

#define PARSER_TOKEN_BUFFER_CAPACITY 64

/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief Parser makes AST, represented by @ref psh::BaseTreeNode and its descendants
 *
 * psh::Parser consumes (but not destroys) psh::Token, usually made by psh::Lexer
 */
class Parser {

    /**
     * @brief Function called on each token push
     */
    typedef int (Parser::*PushTokenFunc)(Token *);

private:

    Token         *tokenBuffer = nullptr;  /**< Stack of tokens*/
    PushTokenFunc pushFunctions[6];        /**< Array of token push functions for each token type */

public:

    explicit Parser();

    ~Parser();

    /**
     * @brief Input function of psh::Parser
     * @param token to accept and push
     * @return push code
     */
    int pushToken(Token *token);

private:

    int pushIdentifierToken(Token *token);

    int pushKeywordToken(Token *token);

    int pushLiteralToken(Token *token);

    int pushOperatorToken(Token *token);

    int pushSeparatorToken(Token *token);

};

}
#endif //PHANTOMSHELL_PARSER_H
