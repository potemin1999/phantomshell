/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/28/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Token.h"
#include "Allocator.h"

using namespace phlib;
using namespace psh;

Allocator *tokenAllocator = Allocator::getDefaultAllocator();

Token::Token(TokenType type,
             int32 line) {
    this->type = type;
    this->line = line;
}


Token::Token(Separator separator,
             int32 line) {
    this->type      = TokenType::SEPARATOR;
    this->separator = separator;
    this->line      = line;
}


Token::Token(Literal literal,
             String *literalValue,
             int32 line) {
    this->type         = TokenType::LITERAL;
    this->literal      = literal;
    this->literalValue = literalValue;
    this->line         = line;
}


Token::Token(Keyword keyword,
             int32 line) {
    this->type    = TokenType::KEYWORD;
    this->keyword = keyword;
    this->line    = line;
}


Token::Token(Operator oper,
             int32 line) {
    this->type = TokenType::OPERATOR;
    this->oper = oper;
    this->line = line;
}


Token::Token(String *identifier,
             int32 line) {
    this->type       = TokenType::IDENTIFIER;
    this->identifier = identifier;
    this->line       = line;
}


Token::~Token() {
    if (type == TokenType::IDENTIFIER)
        delete identifier;
    if (type == TokenType::LITERAL)
        delete literalValue;
    delete stringValue;
}


Ptr Token::operator new(Size size) {
    return tokenAllocator->allocate(size);
}


void Token::operator delete(Ptr tokenPtr) {
    tokenAllocator->deallocate(tokenPtr);
}


const char *Token::tokenToString() {
    delete stringValue;
    this->stringValue = new String("Token(type=");
    String &stringValue = *this->stringValue;
    stringValue += tokenTypeToString(this->type);
    switch (type) {
        case TokenType::IDENTIFIER: {
            stringValue += ",identifier=";
            stringValue += *identifier;
            break;
        }
        case TokenType::KEYWORD: {
            stringValue += ",keyword=";
            stringValue += keywordToString(keyword);
            break;
        }
        case TokenType::LITERAL: {
            stringValue += ",literal=";
            stringValue += literalToString(literal);
            stringValue += ",value=";
            stringValue += *literalValue;
            break;
        }
        case TokenType::OPERATOR: {
            stringValue += ",operator=";
            stringValue += operatorToString(oper);
            break;
        }
        case TokenType::SEPARATOR: {
            stringValue += ",separator=";
            stringValue += separatorToString(separator);
            break;
        }
    }
    stringValue += ",line=";
    stringValue += String::valueOf((int32) line);
    stringValue += ")";
    return stringValue.charValue();
}


const char *Token::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LITERAL: return "LITERAL";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::SEPARATOR: return "SEPARATOR";
        default: return "IDENTIFIER";
    }
}


const char *Token::literalToString(Literal literal) {
    switch (literal) {
        case Literal::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case Literal::STRING_LITERAL: return "STRING_LITERAL";
        case Literal::LOGICAL_LITERAL: return "LOGICAL_LITERAL";
        case Literal::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case Literal::CHARACTER_LITERAL: return "CHARACTER_LITERAL";
    }
    return "UNKNOWN_LITERAL";
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
    }
    return "UNKNOWN_LITERAL";
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
