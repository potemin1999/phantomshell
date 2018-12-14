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

#include "token.h"

#define PARSER_TOKEN_BUFFER_CAPACITY 64

/** @brief Default Shell namespace */
namespace psh {

/**
 */
class Parser{

    typedef int (Parser::*PushTokenFunc)(Token*);

private:

    Token* token_buffer = nullptr;
    PushTokenFunc push_functions[6];

public:

    explicit Parser();

    ~Parser();

    int push_token(Token* token);

private:

    int push_identifier_token(Token* token);

    int push_keyword_token(Token* token);

    int push_literal_token(Token* token);

    int push_operator_token(Token* token);

    int push_separator_token(Token* token);

};

}
#endif //PHANTOMSHELL_PARSER_H
