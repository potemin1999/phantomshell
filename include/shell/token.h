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

    phlib::String *string_value = nullptr;

public:

    Token(TokenType type) {
        this->type = type;
    }

    Token(Separator separator) {
        this->type = TokenType::SEPARATOR;
        this->separator = separator;
    }

    Token(Literal literal, phlib::String *literal_value) {
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

    Token(phlib::String *identifier) {
        this->type = TokenType::IDENTIFIER;
        this->identifier = identifier;
    }

    ~Token() {
        if (type == TokenType::IDENTIFIER)
            delete identifier;
        if (type == TokenType::LITERAL)
            delete literal_value;
        delete string_value;
    }

    Ptr operator new(Size size) {
        return phlib::malloc(size);
    }

    void operator delete(Ptr token_ptr) {
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
            phlib::String* literal_value;/**< valid when psh::Token::type=psh::TokenType::LITERAL */
        };
        Separator separator;             /**< valid when psh::Token::type=psh::TokenType::SEPARATOR */
        Keyword keyword;                 /**< valid when psh::Token::type=psh::TokenType::KEYWORD */
        Operator oper;                   /**< valid when psh::Token::type=psh::TokenType::OPERATOR */
        phlib::String* identifier;       /**< valid when psh::Token::type=psh::TokenType::IDENTIFIER */
    };

    inline bool assert_type(TokenType type) {
        return !(this->type != type);
    }

    const char *token_to_string() {
        delete string_value;
        this->string_value = new phlib::String("Token(type=");
        phlib::String &string_value = *this->string_value;
        string_value += token_type_to_string(this->type);
        switch (type) {
            case TokenType::IDENTIFIER: {
                string_value += ",identifier=";
                string_value += *identifier;
                break;
            }
            case TokenType::KEYWORD: {
                string_value += ",keyword=";
                string_value += keyword_to_string(keyword);
                break;
            }
            case TokenType::LITERAL: {
                string_value += ",literal=";
                string_value += literal_to_string(literal);
                string_value += ",value=";
                string_value += *literal_value;
                break;
            }
            case TokenType::OPERATOR: {
                string_value += ",operator=";
                string_value += operator_to_string(oper);
                break;
            }
            case TokenType::SEPARATOR: {
                string_value += ",separator=";
                string_value += separator_to_string(separator);
                break;
            }
        }
        string_value += ")";
        return string_value.char_value();
    }

    const char *token_type_to_string(TokenType type) {
        switch (type) {
            case TokenType::OPERATOR: return "OPERATOR";
            case TokenType::KEYWORD: return "KEYWORD";
            case TokenType::SEPARATOR: return "SEPARATOR";
            case TokenType::LITERAL: return "LITERAL";
            default: return "IDENTIFIER";
        }
    }

    const char *literal_to_string(Literal literal) {
        switch (literal) {
            case Literal::FLOAT_LITERAL: return "FLOAT_LITERAL";
            case Literal::STRING_LITERAL: return "STRING_LITERAL";
            case Literal::LOGICAL_LITERAL: return "LOGICAL_LITERAL";
            case Literal::INTEGER_LITERAL: return "INTEGER_LITERAL";
            case Literal::CHARACTER_LITERAL: return "CHARACTER_LITERAL";
        }
    }

    const char *separator_to_string(Separator separator) {
        switch (separator) {
            case Separator::DOT: return "DOT";
            case Separator::COMMA: return "COMMA";
            case Separator::SEMICOLON: return "SEMICOLON";
            case Separator::BRACE_OPEN: return "BRACE_OPEN";
            case Separator::BRACE_CLOSE: return "BRACE_CLOSE";
            case Separator::BRACKET_OPEN: return "BRACKET_OPEN";
            case Separator::BRACKET_CLOSE: return "BRACKET_CLOSE";
            case Separator::PARENTHESIS_OPEN: return "PARENTHESIS_OPEN";
            case Separator::PARENTHESIS_CLOSE: return "PARENTHESIS_CLOSE";
        }
    }

};

}
#endif //PHANTOMSHELL_TOKEN_H
