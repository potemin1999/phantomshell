/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/28/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Token.h"
#include "Alloc.h"

using namespace phlib;
using namespace psh;

Allocator *token_allocator = Allocator::getDefaultAllocator();

Token::Token(TokenType type,
             Int32 line) {
    this->type = type;
    this->line = line;
}


Token::Token(Separator separator,
             Int32 line) {
    this->type = TokenType::SEPARATOR;
    this->separator = separator;
    this->line = line;
}


Token::Token(Literal literal,
             String *literalValue,
             Int32 line) {
    this->type = TokenType::LITERAL;
    this->literal = literal;
    this->literalValue = literalValue;
    this->line = line;
}


Token::Token(Keyword keyword,
             Int32 line) {
    this->type = TokenType::KEYWORD;
    this->keyword = keyword;
    this->line = line;
}


Token::Token(Operator oper,
             Int32 line) {
    this->type = TokenType::OPERATOR;
    this->oper = oper;
    this->line = line;
}


Token::Token(String *identifier,
             Int32 line) {
    this->type = TokenType::IDENTIFIER;
    this->identifier = identifier;
    this->line = line;
}


Token::~Token() {
    if (type == TokenType::IDENTIFIER)
        delete identifier;
    if (type == TokenType::LITERAL)
        delete literalValue;
    delete stringValue;
}


Ptr Token::operator new(Size size) {
    return token_allocator->allocate(size);
}

Ptr Token::operator new[](Size size) {
    return Malloc(size);
}

void Token::operator delete(Ptr tokenPtr) {
    token_allocator->deallocate(tokenPtr);
}

void Token::operator delete[](Ptr tokensPtr) {
    return Free(tokensPtr);
}


const char *Token::tokenToString() {
    delete stringValue;
    this->stringValue = new String("Token(type=");
    String &string_value = *this->stringValue;
    string_value += tokenTypeToString(this->type);
    switch (type) {
        case TokenType::IDENTIFIER: {
            string_value += ",identifier=";
            string_value += *identifier;
            break;
        }
        case TokenType::KEYWORD: {
            string_value += ",keyword=";
            string_value += KeywordToString(keyword);
            break;
        }
        case TokenType::LITERAL: {
            string_value += ",literal=";
            string_value += literalToString(literal);
            string_value += ",value=";
            string_value += *literalValue;
            break;
        }
        case TokenType::OPERATOR: {
            string_value += ",operator=";
            string_value += OperatorToString(oper);
            break;
        }
        case TokenType::SEPARATOR: {
            string_value += ",separator=";
            string_value += separatorToString(separator);
            break;
        }
    }
    string_value += ",line=";
    string_value += String::valueOf((Int32) line);
    string_value += ")";
    return string_value.charValue();
}


const char *Token::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LITERAL: return "LITERAL";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::SEPARATOR: return "SEPARATOR";
        default:return "IDENTIFIER";
    }
}


const char *Token::literalToString(Literal literal) {
    switch (literal) {
        case Literal::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case Literal::STRING_LITERAL: return "STRING_LITERAL";
        case Literal::LOGICAL_LITERAL: return "LOGICAL_LITERAL";
        case Literal::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case Literal::CHARACTER_LITERAL: return "CHARACTER_LITERAL";
        default : return "??_LITERAL";
    }
}


const char *Token::separatorToString(Separator separator) {
    switch (separator) {
        case Separator::DOT: return "DOT";
        case Separator::COMMA: return "COMMA";
        case Separator::NEW_LINE: return "NEW_LINE";
        case Separator::SEMICOLON: return "SEMICOLON";
        case Separator::BRACE_OPEN: return "BRACE_OPEN";
        case Separator::BRACE_CLOSE: return "BRACE_CLOSE";
        case Separator::BRACKET_OPEN: return "BRACKET_OPEN";
        case Separator::BRACKET_CLOSE: return "BRACKET_CLOSE";
        case Separator::PARENTHESIS_OPEN: return "PARENTHESIS_OPEN";
        case Separator::PARENTHESIS_CLOSE: return "PARENTHESIS_CLOSE";
        default : return "??_SEPARATOR";
    }
}

bool Token::operator==(const Token &other) {
    if (this->type != other.type) return false;
    switch (this->type) {
        case TokenType::IDENTIFIER: {
            return this->identifier == other.identifier;
        }
        case TokenType::LITERAL: {
            return this->literal == other.literal &&
                   this->literalValue == other.literalValue;
        }
        case TokenType::KEYWORD: {
            return this->keyword == other.keyword;
        }
        case TokenType::OPERATOR: {
            return this->oper == other.oper;
        }
        case TokenType::SEPARATOR: {
            return this->separator == other.separator;
        }
        default: {
            return false;
        }
    }
}
