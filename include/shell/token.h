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

#include <phlib/string.h>
#include "keyword.h"
#include "operator.h"

/** @brief Default Shell namespace */
namespace psh {

typedef enum Literal{
    LOGICAL_LITERAL   = 0x01,
    INTEGER_LITERAL   = 0x02,
    FLOAT_LITERAL     = 0x03,
    CHARACTER_LITERAL = 0x04,
    STRING_LITERAL    = 0x05
} Literal;

typedef enum Separator{
    DOT               = 0x00,
    COMMA             = 0x01,
    SEMICOLON         = 0x02,
    PARENTHESIS_OPEN  = 0x08,
    PARENTHESIS_CLOSE = 0x09,
    BRACKET_OPEN      = 0x0a,
    BRACKET_CLOSE     = 0x0b,
    BRACE_OPEN        = 0x0c,
    BRACE_CLOSE       = 0x0d
} Separator;

typedef enum TokenType {
    IDENTIFIER    = 0x01, /**< described by identifier field */
    OPERATOR      = 0x02, /**< described by oper field */
    KEYWORD       = 0x03, /**< described by keyword field */
    SEPARATOR     = 0x04, /**< described by separator field */
    LITERAL       = 0x05, /**< described by literal and literal_value fields */
} TokenType;

/**
 * @brief Describes output objects of lexer
 */
class Token {

private:

    phlib::String *string_value;

public:

    Token(TokenType type) {
        this->type = type;
    }

    Token(Separator separator){
        this->type = TokenType::SEPARATOR;
        this->separator = separator;
    }

    Token(Literal literal,const char* literal_value){
        this->type = TokenType::LITERAL;
        this->literal = literal;
        this->literal_value = literal_value;
    }

    Token(Keyword keyword) {
        this->type = TokenType::KEYWORD;
        this->keyword = keyword;
    }

    Token(Operator oper) {
        this->type = TokenType::OPERATOR;
        this->oper = oper;
    }

    Token(const char16 *identifier) {
        this->type = TokenType::IDENTIFIER;
        this->identifier = identifier;
    }

    ~Token() {
        delete string_value;
    }

    Ptr operator new(Size size){
        return phlib::malloc(size);
    }

    void operator delete(Ptr token_ptr){
        phlib::free(token_ptr);
    }

    /**
     * @brief token used always and shows how to interpret other data
     *
     * It determines which data accessed from union will be valid
     */
    TokenType type;
    union {
        /**
         * if psh::Token::type equal to psh::TokenType::LITERAL,
         * this struct will be used to describe literal, its type and value
         */
        struct{
            Literal literal;             /**< valid when psh::Token::type=psh::TokenType::LITERAL */
            const char* literal_value;   /**< valid when psh::Token::type=psh::TokenType::LITERAL */
        };
        Separator separator;             /**< valid when psh::Token::type=psh::TokenType::SEPARATOR */
        Keyword keyword;                 /**< valid when psh::Token::type=psh::TokenType::KEYWORD */
        Operator oper;                   /**< valid when psh::Token::type=psh::TokenType::OPERATOR */
        const char16 *identifier;          /**< valid when psh::Token::type=psh::TokenType::IDENTIFIER */
    };

    inline bool assert_type(TokenType type) {
        return !(this->type != type);
    }

private:

    const char* token_type_to_string(TokenType type){
        switch (type){
            case TokenType::OPERATOR: return "OPERATOR";
            case TokenType::KEYWORD: return "KEYWORD";
            case TokenType::SEPARATOR: return "SEPARATOR";
            case TokenType::LITERAL: return "LITERAL";
            default: return "IDENTIFIER";
        }
    }

};

}
#endif //PHANTOMSHELL_TOKEN_H
