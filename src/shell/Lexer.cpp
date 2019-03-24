/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/17/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Lexer.h"

using namespace psh;
using namespace phlib;


Lexer::Lexer(IStream *inputStream) {
    istream            = inputStream;
    readBufferPointer  = LEXER_READ_BUFFER_SIZE;
    readBufferSize     = LEXER_READ_BUFFER_SIZE;
    stashBufferSize    = LEXER_STASH_BUFFER_SIZE;
    readBuffer = new UInt8[LEXER_READ_BUFFER_SIZE];
    //readBuffer         = (UInt8 *) phlib::malloc(LEXER_READ_BUFFER_SIZE);
    stashBuffer        = (Symbol *) phlib::malloc(LEXER_STASH_BUFFER_SIZE);
    stashBufferPointer = 0;
    currentLine        = 1;
}


Lexer::~Lexer() {
    phlib::free(readBuffer);
    phlib::free(stashBuffer);
}


Lexer::Symbol Lexer::readNextSymbol() {
    if (readBufferPointer >= readBufferSize) {
        readBufferSize = (UInt32) updateBuffer();
        if (readBufferSize == 0) return '\0';
        readBufferPointer = 0;
    }
    UInt8 hi = readBuffer[readBufferPointer++];
    if ((hi & 0b10000000) == 0x00000000) return hi;
    UInt8 lo = readBuffer[readBufferPointer++];
    return ((hi << 8) & lo);
}


Lexer::Symbol Lexer::getNextSymbol() {
    if (stashBufferPointer != 0) {
        stashBufferPointer -= 1;
        auto retValue = stashBuffer[stashBufferPointer];
        return retValue;
    }
    auto symbol = readNextSymbol();
    return symbol;
}


void Lexer::stashSymbol(Symbol symbol) {
    stashBuffer[stashBufferPointer] = symbol;
    stashBufferPointer += 1;
}


SSize Lexer::updateBuffer() {
    return istream->read(readBuffer, readBufferSize);
}


String *Lexer::createFromStashBuffer() {
    if (stashBufferPointer == 0)
        return new String();
    auto retStr = new String(stashBuffer, (Size) stashBufferPointer);
    stashBufferPointer = 0;
    return retStr;
}


Operator Lexer::checkIfIdentifierIsOperator() {
    if (stashBufferPointer != 2 &
        stashBufferPointer != 3) {
        return (Operator) 0;
    }
    switch (stashBuffer[0]) {
        case 'a': {
            if (stashBufferPointer != 3)
                return (Operator) 0;
            if (stashBuffer[1] == 'n' &
                stashBuffer[2] == 'd') {
                stashBufferPointer -= 3;
                return Operator::LOGICAL_AND;
            } else {
                return (Operator) 0;
            }
        }
        case 'n': {
            if (stashBufferPointer != 3)
                return (Operator) 0;
            if (stashBuffer[1] == 'o' &
                stashBuffer[2] == 't') {
                stashBufferPointer -= 3;
                return Operator::LOGICAL_NOT;
            } else {
                return (Operator) 0;
            }
        }
        case 'o': {
            if (stashBufferPointer != 2)
                return (Operator) 0;
            if (stashBuffer[1] == 'r') {
                stashBufferPointer -= 2;
                return Operator::LOGICAL_OR;
            } else {
                return (Operator) 0;
            }
        }
        case 'x': {
            if (stashBufferPointer != 3)
                return (Operator) 0;
            if (stashBuffer[1] == 'o' &
                stashBuffer[2] == 'r') {
                stashBufferPointer -= 3;
                return Operator::LOGICAL_XOR;
            } else {
                return (Operator) 0;
            }
        }
        default: {
            return (Operator) 0;
        }
    }
}


inline Token *Lexer::makeOperatorToken(Symbol char1) {
    Operator oper = makeOperator(char1);
    if (oper != 0) {
        return new Token(oper, currentLine);
    } else {
        return nullptr;
    }
}


Operator Lexer::makeOperator(Lexer::Symbol char1) {
    if (char1 == '+') {
        Symbol char2 = getNextSymbol();
        if (char2 == '+') {
            return Operator::POST_INCREMENT;
        } else {
            stashSymbol(char2);
            return Operator::ADDITION;
        }
    }
    if (char1 == '-') {
        Symbol char2 = getNextSymbol();
        if (char2 == '-') {
            return Operator::POST_DECREMENT;
        } else if (char2 == '>') {
            return Operator::LOGICAL_IMPLICATION;
        } else {
            stashSymbol(char2);
            return Operator::SUBTRACTION;
        }
    }
    if (char1 == '=') {
        Symbol char2 = getNextSymbol();
        if (char2 == '=') {
            return Operator::EQUAL_TO;
        } else {
            stashSymbol(char2);
            return Operator::ASSIGNMENT;
        }
    }
    if (char1 == '~') {
        return Operator::BITWISE_NOT;
    }
    if (char1 == '!') {
        Symbol char2 = getNextSymbol();
        if (char2 == '=') {
            return Operator::NOT_EQUAL_TO;
        } else {
            stashSymbol(char2);
            return Operator::LOGICAL_NOT;
        }
    }
    if (char1 == '*') {
        return Operator::MULTIPLICATION;
    }
    if (char1 == '/') {
        Symbol char2 = getNextSymbol();
        if (char2 == '\\') {
            return Operator::BITWISE_AND;
        } else {
            stashSymbol(char2);
            return Operator::DIVISION;
        }
    }
    if (char1 == '<') {
        Symbol char2 = getNextSymbol();
        if (char2 == '<') {
            return Operator::BITWISE_SHIFT_LEFT;
        } else if (char2 == '=') {
            return Operator::NOT_GREATER_THAN;
        } else {
            stashSymbol(char2);
            return Operator::LESS_THAN;
        }
    }
    if (char1 == '>') {
        Symbol char2 = getNextSymbol();
        if (char2 == '>') {
            return Operator::BITWISE_SHIFT_RIGHT;
        } else if (char2 == '=') {
            return Operator::NOT_LESS_THAN;
        } else {
            stashSymbol(char2);
            return Operator::GREATER_THAN;
        }
    }
    if (char1 == '\\') {
        Symbol char2 = getNextSymbol();
        if (char2 == '\'') {
            Symbol char3 = getNextSymbol();
            if (char3 == '/') {
                return Operator::BITWISE_XOR;
            } else {
                stashSymbol(char3);
            }
        } else if (char2 == '/') {
            return Operator::BITWISE_OR;
        } else {
            stashSymbol(char2);
        }
    }
    return (Operator) 0;
}


Separator Lexer::makeSeparator(Lexer::Symbol symbol) {
    if (symbol == '(' | symbol == ')') {
        return symbol == '(' ?
               Separator::PARENTHESIS_OPEN :
               Separator::PARENTHESIS_CLOSE;
    }
    if (symbol == '{' | symbol == '}') {
        return symbol == '{' ?
               Separator::BRACE_OPEN :
               Separator::BRACE_CLOSE;
    }
    if (symbol == '[' | symbol == ']') {
        return symbol == '[' ?
               Separator::BRACKET_OPEN :
               Separator::BRACE_CLOSE;
    }
    if (symbol == '.') {
        return Separator::DOT;
    }
    if (symbol == ';') {
        return Separator::SEMICOLON;
    }
    if (symbol == '\n') {
        return Separator::NEW_LINE;
    }
    if (symbol == ',') {
        return Separator::COMMA;
    }
    return (Separator) 0;
}


Token *Lexer::getNextToken() {
    Symbol char1;
    do {
        char1 = getNextSymbol();
    } while (char1 == ' ');
    if (char1 == '\0') return nullptr;
    auto  isCommentSkipped = false;
    if (char1 == '/') {
        Symbol char2 = getNextSymbol();
        if (char2 == '/') {
            while (readNextSymbol() != '\n') {}
            isCommentSkipped = true;
        } else if (char2 == '*') {
            Symbol lastRead;
            bool   lastReadAsterisk = false;
            while (!(lastReadAsterisk & ((lastRead = readNextSymbol()) == '/'))) {
                lastReadAsterisk = lastRead == '*';
            }

            isCommentSkipped = true;
        } else {
            stashSymbol(char2);
        }
    }
    if (isCommentSkipped) {
        char1 = getNextSymbol();
        if (char1 == '\0') return nullptr;
        while (char1 == ' ') {
            char1 = getNextSymbol();
        }
    }
    if (Character::isLetter(char1)) {
        stashSymbol(char1);
        Symbol lastRead;
        bool   mayBeKeyword = true;
        bool   lastReadIsLetter;
        bool   lastReadIsDigit;
        bool   lastReadIsUnderscore;
        do {
            lastRead             = readNextSymbol();
            lastReadIsDigit      = Character::isDigit(lastRead);
            lastReadIsLetter     = Character::isLetter(lastRead);
            lastReadIsUnderscore = lastRead == '_';
            stashSymbol(lastRead);
            if (!lastReadIsLetter &
                (lastReadIsDigit || lastReadIsUnderscore)) {
                mayBeKeyword = false;
            }
        } while (lastReadIsLetter |
                 lastReadIsDigit |
                 lastReadIsUnderscore);
        stashBufferPointer -= 1;
        UInt32 oper         = checkIfIdentifierIsOperator();
        if (oper != 0) {
            auto retToken = new Token((Operator) oper, currentLine);
            stashSymbol(lastRead);
            return retToken;
        }
        String *stringValue = createFromStashBuffer();
        stashSymbol(lastRead);
        int keywordValue = -1;
        if (mayBeKeyword) {
            keywordValue = isKeyword(*stringValue);
        }
        DEBUG_LOG_LEX("is keyword ? %s\n", keywordValue != -1 ? "true" : "false");
        if (keywordValue != -1) {
            delete stringValue;
            return new Token((Keyword) keywordValue, currentLine);
        } else {
            return new Token(stringValue, currentLine);
        }
    }
    //if we are starting from digits
    if (Character::isDigit(char1)) {
        stashSymbol(char1);
        Symbol lastRead;
        bool   hasDot = false;
        bool   lastReadIsDot;
        bool   lastReadIsDigit;
        do {
            lastRead        = readNextSymbol();
            lastReadIsDigit = Character::isDigit(lastRead);
            lastReadIsDot   = lastRead == '.';
            //TODO: hasDot & lastReadIsDot should be invalid
            hasDot          = hasDot | lastReadIsDot;
            stashSymbol(lastRead);
        } while (lastReadIsDigit |
                 lastReadIsDot);
        stashBufferPointer -= 1;
        if (lastReadIsDot) {
            //last symbol is dot. what should we do
        }
        auto literalValue = createFromStashBuffer();
        stashSymbol(lastRead);
        return new Token(hasDot ? Literal::FLOAT_LITERAL : Literal::INTEGER_LITERAL,
                         literalValue, currentLine);
    }
    if (char1 == '"') {
        Symbol lastRead;
        bool   lastReadEscape;
        bool   lastReadDoubleQuotes;
        do {
            lastRead             = readNextSymbol();
            lastReadEscape       = lastRead == '\\';
            lastReadDoubleQuotes = lastRead == '"';
            stashSymbol(lastRead);
        } while (!lastReadDoubleQuotes |
                 lastReadEscape);
        stashBufferPointer -= 1;
        auto stringLiteralValue = createFromStashBuffer();
        return new Token(Literal::STRING_LITERAL, stringLiteralValue, currentLine);
    }
    Token *operator_token  = makeOperatorToken(char1);
    if (operator_token != nullptr) {
        return operator_token;
    }
    // only non-digits and non-letters left
    Separator separator = makeSeparator(char1);
    if (separator != 0) {
        auto token = new Token(separator, currentLine);
        if (token->separator == Separator::NEW_LINE)
            currentLine++;
        return token;
    } else {
        return nullptr;
    }
}

