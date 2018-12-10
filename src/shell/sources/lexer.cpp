/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/17/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "lexer.h"

using namespace psh;
using namespace phlib;


Lexer::Lexer(IStream *input_stream) {
    istream = input_stream;
    read_buffer_pointer = LEXER_READ_BUFFER_SIZE;
    read_buffer_size = LEXER_READ_BUFFER_SIZE;
    read_buffer = (uint8 *) phlib::malloc(LEXER_READ_BUFFER_SIZE);
    stash_buffer_pointer = 0;
    stash_buffer_size = LEXER_STASH_BUFFER_SIZE;
    stash_buffer = (Symbol *) phlib::malloc(LEXER_STASH_BUFFER_SIZE);
}


Lexer::~Lexer() {
    phlib::free(read_buffer);
    phlib::free(stash_buffer);
}


psh::Lexer::Symbol Lexer::read_next_symbol() {
    if (read_buffer_pointer >= read_buffer_size) {
        read_buffer_size = (uint32) update_buffer();
        if (read_buffer_size == 0) return '\0';
        read_buffer_pointer = 0;
    }
    uint8 hi = read_buffer[read_buffer_pointer++];
    if ((hi & 0b10000000) == 0x00000000) return hi;
    uint8 lo = read_buffer[read_buffer_pointer++];
    return ((hi << 8) & lo);
}


psh::Lexer::Symbol Lexer::get_next_symbol() {
    if (stash_buffer_pointer != 0) {
        stash_buffer_pointer -= 1;
        Symbol ret_value = stash_buffer[stash_buffer_pointer];
        return ret_value;
    }
    Symbol symbol = read_next_symbol();
    return symbol;
}


void Lexer::stash_symbol(Symbol symbol) {
    stash_buffer[stash_buffer_pointer] = symbol;
    stash_buffer_pointer += 1;
}


SSize Lexer::update_buffer() {
    return istream->read(read_buffer, read_buffer_size);
}


phlib::String *Lexer::create_from_stash_buffer() {
    if (stash_buffer_pointer == 0) return new String();
    auto ret_str = new String(stash_buffer, (Size) stash_buffer_pointer);
    stash_buffer_pointer = 0;
    return ret_str;
}


uint32 Lexer::check_if_identifier_is_operator() {
    if (stash_buffer_pointer != 2 &
        stash_buffer_pointer != 3) {
        return 0;
    }
    switch (stash_buffer[0]) {
        case 'a': {
            if (stash_buffer_pointer != 3)
                return 0;
            if (stash_buffer[1] == 'n' &
                stash_buffer[2] == 'd') {
                stash_buffer_pointer -= 3;
                return Operator::LOGICAL_AND;
            } else {
                return 0;
            }
        }
        case 'n': {
            if (stash_buffer_pointer != 3)
                return 0;
            if (stash_buffer[1] == 'o' &
                stash_buffer[2] == 't') {
                stash_buffer_pointer -= 3;
                return Operator::LOGICAL_NOT;
            } else {
                return 0;
            }
        }
        case 'o': {
            if (stash_buffer_pointer != 2)
                return 0;
            if (stash_buffer[1] == 'r') {
                stash_buffer_pointer -= 2;
                return Operator::LOGICAL_OR;
            } else {
                return 0;
            }
        }
        case 'x': {
            if (stash_buffer_pointer != 3)
                return 0;
            if (stash_buffer[1] == 'o' &
                stash_buffer[2] == 'r') {
                stash_buffer_pointer -= 3;
                return Operator::LOGICAL_XOR;
            } else {
                return 0;
            }
        }
        default: {
            return 0;
        }
    }
}


Token *Lexer::get_next_token() {
    Symbol char_1;
    do {
        char_1 = get_next_symbol();
    } while (char_1 == ' ');
    if (char_1 == '\0') return nullptr;
    if (Character::is_letter(char_1)) {
        stash_symbol(char_1);
        Symbol last_read;
        bool may_be_keyword = true;
        bool last_read_is_letter;
        bool last_read_is_digit;
        bool last_read_is_underscore;
        do {
            last_read = read_next_symbol();
            last_read_is_digit = Character::is_digit(last_read);
            last_read_is_letter = Character::is_letter(last_read);
            last_read_is_underscore = last_read == '_';
            stash_symbol(last_read);
            if (!last_read_is_letter &
                (last_read_is_digit || last_read_is_underscore)) {
                may_be_keyword = false;
            }
        } while (last_read_is_letter |
                 last_read_is_digit |
                 last_read_is_underscore);
        stash_buffer_pointer -= 1;
        uint32 oper = check_if_identifier_is_operator();
        if (oper != 0) {
            auto ret_token = new Token((Operator) oper);
            stash_symbol(last_read);
            return ret_token;
        }
        String *string_value = create_from_stash_buffer();
        stash_symbol(last_read);
        int keyword_value = -1;
        if (may_be_keyword) {
            keyword_value = is_keyword(*string_value);
        }
        DEBUG_LOG_LEX("is keyword ? %s\n", keyword_value != -1 ? "true" : "false");
        if (keyword_value != -1) {
            delete string_value;
            return new Token((Keyword) keyword_value);
        } else {
            return new Token(string_value);
        }
    }
    //if we are starting from digits
    if (Character::is_digit(char_1)) {
        stash_symbol(char_1);
        Symbol last_read;
        bool has_dot = false;
        bool last_read_is_dot;
        bool last_read_is_digit;
        do {
            last_read = read_next_symbol();
            last_read_is_digit = Character::is_digit(last_read);
            last_read_is_dot = last_read == '.';
            has_dot = has_dot | last_read_is_dot;
            stash_symbol(last_read);
        } while (last_read_is_digit |
                 last_read_is_dot);
        stash_buffer_pointer -= 1;
        if (last_read_is_dot) {
            //last symbol is dot. what should we do
        }
        auto literal_value = create_from_stash_buffer();
        stash_symbol(last_read);
        if (has_dot) {
            return new Token(Literal::FLOAT_LITERAL, literal_value);
        } else {
            return new Token(Literal::INTEGER_LITERAL, literal_value);
        }
    }
    if (char_1 == '"') {
        Symbol last_read;
        bool last_read_escape;
        bool last_read_double_quotes;
        do {
            last_read = read_next_symbol();
            last_read_escape = last_read == '\\';
            last_read_double_quotes = last_read == '"';
            stash_symbol(last_read);
        } while (!last_read_double_quotes |
                 last_read_escape);
        stash_buffer_pointer -= 1;
        auto string_literal_value = create_from_stash_buffer();
        return new Token(Literal::STRING_LITERAL, string_literal_value);
    }
    // only non-digits and non-letters left
    if (char_1 == '(' | char_1 == ')') {
        return new Token(char_1 == '(' ?
                         Separator::PARENTHESIS_OPEN :
                         Separator::PARENTHESIS_CLOSE);
    }
    if (char_1 == '{' | char_1 == '}') {
        return new Token(char_1 == '{' ?
                         Separator::BRACE_OPEN :
                         Separator::BRACE_CLOSE);
    }
    if (char_1 == '[' | char_1 == ']') {
        return new Token(char_1 == '[' ?
                         Separator::BRACKET_OPEN :
                         Separator::BRACE_CLOSE);
    }
    if (char_1 == '.') {
        return new Token(Separator::DOT);
    }
    if (char_1 == ';') {
        return new Token(Separator::SEMICOLON);
    }
    if (char_1 == '\n') {
        return new Token(Separator::SEMICOLON);
    }
    if (char_1 == ',') {
        return new Token(Separator::COMMA);
    }
}
