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

#include "String.h"
#include "Keyword.h"
#include "Operator.h"

/** @brief Default Shell namespace */
namespace psh {

typedef enum Literal {
    LOGICAL_LITERAL   = 0x01,
    INTEGER_LITERAL   = 0x02,
    FLOAT_LITERAL     = 0x03,
    CHARACTER_LITERAL = 0x04,
    STRING_LITERAL    = 0x05
} Literal;

typedef enum Separator {
    DOT               = 0x04,
    COMMA             = 0x01,
    SEMICOLON         = 0x02,
    NEW_LINE          = 0x03,
    PARENTHESIS_OPEN  = 0x08,
    PARENTHESIS_CLOSE = 0x09,
    BRACKET_OPEN      = 0x0a,
    BRACKET_CLOSE     = 0x0b,
    BRACE_OPEN        = 0x0c,
    BRACE_CLOSE       = 0x0d
} Separator;

typedef enum TokenType {
    IDENTIFIER = 0x01, /**< described by identifier field */
    OPERATOR   = 0x02, /**< described by oper field */
    KEYWORD    = 0x03, /**< described by keyword field */
    SEPARATOR  = 0x04, /**< described by separator field */
    LITERAL    = 0x05, /**< described by literal and literal_value fields */
} TokenType;

/**
 * @brief Describes output objects of lexer
 */
class Token {

private:

    phlib::String *stringValue = nullptr;

public:

    Token(TokenType type, int32 line);

    Token(Separator separator, int32 line);

    Token(Literal literal, phlib::String *literalValue, int32 line);

    Token(Keyword keyword, int32 line);

    Token(Operator oper, int32 line);

    Token(phlib::String *identifier, int32 line);

    ~Token();

    Ptr operator new(Size size);

    void operator delete(Ptr tokenPtr);

    /**
     * @brief token used always and shows how to interpret other data
     *
     * It determines which data accessed from union will be valid
     */
    TokenType type;
    int32     line;
    union {
        /**
         * if psh::Token::type equal to psh::TokenType::LITERAL,
         * this struct will be used to describe literal, its type and value
         */
        struct {
            Literal       literal;       /**< valid when psh::Token::type=psh::TokenType::LITERAL */
            phlib::String *literalValue; /**< valid when psh::Token::type=psh::TokenType::LITERAL */
        };
        Separator separator;              /**< valid when psh::Token::type=psh::TokenType::SEPARATOR */
        Keyword   keyword;                /**< valid when psh::Token::type=psh::TokenType::KEYWORD */
        Operator  oper;                   /**< valid when psh::Token::type=psh::TokenType::OPERATOR */

        phlib::String *identifier;        /**< valid when psh::Token::type=psh::TokenType::IDENTIFIER */
    };

    inline bool assertType(TokenType type) {
        return !(this->type != type);
    }

    const char *tokenToString();

    const char *tokenTypeToString(TokenType type);

    const char *literalToString(Literal literal);

    const char *separatorToString(Separator separator);

    bool operator==(const Token &other);

};

}
#endif //PHANTOMSHELL_TOKEN_H
