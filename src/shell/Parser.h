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

#include "Keyword.h"
#include "Token.h"
#include "Tree.h"

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
    class ParserData;

private:

    ParserData* data;

public:

    explicit Parser();

    ~Parser();

    /**
     * @brief Input function of psh::Parser
     * @param token to accept and push
     * @return push code
     */
    int pushToken(Token *token);

};

}
#endif //PHANTOMSHELL_PARSER_H
