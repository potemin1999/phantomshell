/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/28/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "token.h"
#include "alloc.h"

using namespace phlib;


phlib::Allocator *token_allocator = phlib::Allocator::get_default_allocator();

psh::Token::Token(TokenType type) {
    this->type = type;
}


psh::Token::Token(Separator separator) {
    this->type = TokenType::SEPARATOR;
    this->separator = separator;
}


psh::Token::Token(Literal literal, String *literal_value) {
    this->type = TokenType::LITERAL;
    this->literal = literal;
    this->literal_value = literal_value;
}


psh::Token::Token(Keyword keyword) {
    this->type = TokenType::KEYWORD;
    this->keyword = keyword;
}


psh::Token::Token(Operator oper) {
    this->type = TokenType::OPERATOR;
    this->oper = oper;
}


psh::Token::Token(String *identifier) {
    this->type = TokenType::IDENTIFIER;
    this->identifier = identifier;
}


psh::Token::~Token() {
    if (type == TokenType::IDENTIFIER)
        delete identifier;
    if (type == TokenType::LITERAL)
        delete literal_value;
    delete string_value;
}


Ptr psh::Token::operator new(Size size) {
    return token_allocator->allocate(size);
}


void psh::Token::operator delete(Ptr token_ptr) {
    token_allocator->deallocate(token_ptr);
}


const char *psh::Token::token_to_string() {
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


const char *psh::Token::token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::OPERATOR:
            return "OPERATOR";
        case TokenType::KEYWORD:
            return "KEYWORD";
        case TokenType::SEPARATOR:
            return "SEPARATOR";
        case TokenType::LITERAL:
            return "LITERAL";
        default:
            return "IDENTIFIER";
    }
}


const char *psh::Token::literal_to_string(Literal literal) {
    switch (literal) {
        case Literal::FLOAT_LITERAL:
            return "FLOAT_LITERAL";
        case Literal::STRING_LITERAL:
            return "STRING_LITERAL";
        case Literal::LOGICAL_LITERAL:
            return "LOGICAL_LITERAL";
        case Literal::INTEGER_LITERAL:
            return "INTEGER_LITERAL";
        case Literal::CHARACTER_LITERAL:
            return "CHARACTER_LITERAL";
    }
}


const char *psh::Token::separator_to_string(Separator separator) {
    switch (separator) {
        case Separator::DOT:
            return "DOT";
        case Separator::COMMA:
            return "COMMA";
        case Separator::SEMICOLON:
            return "SEMICOLON";
        case Separator::BRACE_OPEN:
            return "BRACE_OPEN";
        case Separator::BRACE_CLOSE:
            return "BRACE_CLOSE";
        case Separator::BRACKET_OPEN:
            return "BRACKET_OPEN";
        case Separator::BRACKET_CLOSE:
            return "BRACKET_CLOSE";
        case Separator::PARENTHESIS_OPEN:
            return "PARENTHESIS_OPEN";
        case Separator::PARENTHESIS_CLOSE:
            return "PARENTHESIS_CLOSE";
    }
}
