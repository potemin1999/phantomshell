/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/18/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_TOKEN_H
#define PHANTOMSHELL_TOKEN_H

#include "keyword.h"
#include "operator.h"

/** @brief Default Shell namespace */
namespace psh {

typedef enum TokenType {
    IDENTIFIER    = 0x01, /**< described by identifier field */
    OPERATOR      = 0x02, /**< described by oper field */
    KEYWORD       = 0x03, /**< described by identifier field */
} TokenType;

/**
 * @brief Describes output objects of lexer
 */
class Token {

public:

    Token(TokenType type) {
        this->type = type;
    }

    Token(Keyword keyword) {
        this->type = TokenType::KEYWORD;
        this->keyword = keyword;
    }

    Token(Operator oper) {
        this->type = TokenType::OPERATOR;
        this->oper = oper;
    }

    Token(const char *identifier) {
        this->type = TokenType::IDENTIFIER;
        this->identifier = identifier;
    }

    ~Token() {

    }

    TokenType type;
    union {
        union {
            bool is_valid = true;
            Keyword keyword;
            Operator oper;
        };
        const char *identifier;
    };

    inline bool assert_type(TokenType type) {
        return !(this->type != type);
    }

};

}
#endif //PHANTOMSHELL_TOKEN_H
