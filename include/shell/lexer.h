/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/14/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_LEXER_H
#define PHANTOMSHELL_LEXER_H

#include <phlib/alloc.h>
#include <phlib/character.h>
#include <phlib/istream.h>
#include <shell/token.h>

#define LEXER_READ_BUFFER_SIZE 8192
#define LEXER_TOKEN_BUFFER_SIZE 256

/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief Lexer translates symbols from @ref phlib::IStream into @ref Token
 */
class Lexer {

private:

    phlib::IStream *istream;
    uint8 *read_buffer;
    uint32 read_buffer_pointer;
    uint32 read_buffer_size;
    uint8 *token_buffer;
    uint32 token_buffer_pointer;
    uint32 token_buffer_size;

public:

    Lexer(phlib::IStream *istream);

    ~Lexer();

    void *operator new(Size size) {
        return phlib::malloc(size);
    }

    void operator delete(Ptr ptr) {
        phlib::free(ptr);
    }

    Token *get_next_token();

private:

    char16 get_next_char();

    char16 read_next_char();

    SSize update_buffer();

};

} //namespace psh

#endif //PHANTOMSHELL_LEXER_H
