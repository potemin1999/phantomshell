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
    token_buffer_pointer = LEXER_TOKEN_BUFFER_SIZE;
    token_buffer_size = LEXER_TOKEN_BUFFER_SIZE;
    token_buffer = (uint8 *) phlib::malloc(LEXER_TOKEN_BUFFER_SIZE);
}

Lexer::~Lexer() {
    phlib::free(read_buffer);
    phlib::free(token_buffer);
}

char16 Lexer::read_next_char() {
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

char16 Lexer::get_next_char() {
    return read_next_char();
}

SSize Lexer::update_buffer() {
    return istream->read(read_buffer, read_buffer_size);
}

Token *Lexer::get_next_token() {
    while (true) {
        char16 char1 = get_next_char();
        bool is = (char1 == '\0');
        if (is)
            break;
        DEBUG_LOG("%c\n", (char) char1);
        if (Character::is_letter(char1)) {

        } else {

        }
    }
    return nullptr;
}
