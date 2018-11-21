/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/17/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <shell/lexer.h>

using namespace psh;
using namespace phlib;

Lexer::Lexer(IStream *input_stream) {
    istream = input_stream;
    read_buffer_pointer = LEXER_READ_BUFFER_SIZE;
    read_buffer_size = LEXER_READ_BUFFER_SIZE;
    read_buffer = (uint8 *) phlib::malloc(LEXER_READ_BUFFER_SIZE);
    token_buffer_pointer = 0;
    token_buffer_size = LEXER_TOKEN_BUFFER_SIZE;
    token_buffer = (Symbol *) phlib::malloc(LEXER_TOKEN_BUFFER_SIZE);
    stashed_symbol = 0;
}

Lexer::~Lexer() {
    phlib::free(read_buffer);
    phlib::free(token_buffer);
}

psh::Lexer::Symbol Lexer::read_next_symbol() {
    if (read_buffer_pointer == read_buffer_size) {
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
    if (stashed_symbol!= 0){
        --stashed_symbol;
        token_buffer_pointer = (token_buffer_pointer + 1) % token_buffer_size;
        return token_buffer[(token_buffer_pointer-1) % token_buffer_size];
    }
    Symbol symbol = read_next_symbol();
    token_buffer[token_buffer_pointer] = symbol;
    token_buffer_pointer = (token_buffer_pointer + 1) % token_buffer_size;
    return symbol;
}

void Lexer::stash_symbol(Symbol symbol) {
    token_buffer_pointer = (token_buffer_pointer+token_buffer_size-1) % token_buffer_size;
    token_buffer[token_buffer_pointer] = symbol;
    ++stashed_symbol;
}

SSize Lexer::update_buffer() {
    return istream->read(read_buffer, read_buffer_size);
}

Token *Lexer::get_next_token() {
    Symbol char_1;
    do {
        char_1 = get_next_symbol();
    } while (char_1 == ' ');
    if (char_1 == '\0') return nullptr;
    DEBUG_LOG("%c\n", (char) char_1);
    if (Character::is_letter(char_1)) {
        uint32 start_index = token_buffer_pointer - 1;
        Symbol last_read;
        bool may_be_keyword = true;
        bool last_read_is_letter;
        bool last_read_is_digit;
        bool last_read_is_underscore;
        do {
            last_read = get_next_symbol();
            last_read_is_digit = Character::is_digit(last_read);
            last_read_is_letter = Character::is_letter(last_read);
            last_read_is_underscore = last_read == '_';
            if (!last_read_is_letter &
                (last_read_is_digit || last_read_is_underscore)) {
                may_be_keyword = false;
            }
        } while (last_read_is_letter |
                 last_read_is_digit |
                 last_read_is_underscore);
        uint32 end_index = token_buffer_pointer - 1;
        stash_symbol(last_read);
        String string_value = String(token_buffer+start_index,end_index-start_index);
        DEBUG_LOG("string lexed : %s\n",string_value.char_value());
        DEBUG_LOG("may be keyword ? %s\n",may_be_keyword ? "true" : "false");
        if (may_be_keyword) {
            return new Token(TokenType::KEYWORD);
        }else{
            return new Token(string_value.value());
        }
    }
    //if we are starting from digits
    if (Character::is_digit(char_1)){
        uint32 start_index = token_buffer_pointer - 1;
        Symbol last_read;
        bool has_dot = false;
        bool last_read_is_dot;
        bool last_read_is_digit;
        do{
            last_read = get_next_symbol();
            last_read_is_digit = Character::is_digit(last_read);
            last_read_is_dot = last_read == '.';
            has_dot = has_dot | last_read_is_dot;
        }while (last_read_is_digit |
                last_read_is_dot);
        uint32 end_index = token_buffer_pointer - 1;
        stash_symbol(last_read);
        if (last_read_is_dot){
            //last symbol is dot. what should we do
        }
        String literal_value = String(token_buffer+start_index,end_index-start_index);
        if (has_dot){
            return new Token(Literal::FLOAT_LITERAL,literal_value);
        }else{
            return new Token(Literal::INTEGER_LITERAL,literal_value);
        }
    }
    // only non-digits and non-letters left
    if (char_1 == '(' | char_1 == ')'){
        return new Token(char_1 == '(' ?
                         Separator::PARENTHESIS_OPEN :
                         Separator::PARENTHESIS_CLOSE);
    }
    if (char_1 == '{' | char_1 == '}'){
        return new Token(char_1 == '{' ?
                        Separator::BRACE_OPEN :
                        Separator::BRACE_CLOSE);
    }
    if (char_1 == '[' | char_1 == ']'){
        return new Token(char_1 == '[' ?
                        Separator::BRACKET_OPEN :
                        Separator::BRACE_CLOSE);
    }
    if (char_1 == '.'){
        return new Token(Separator::DOT);
    }
    if (char_1 == ';'){
        return new Token(Separator::SEMICOLON);
    }
    if (char_1 == ','){
        return new Token(Separator::COMMA);
    }
}
