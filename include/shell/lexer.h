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

    /**
     * @brief if we want to change or redefine symbol, we can do it here
     */
    typedef char16 Symbol;

private:

    phlib::IStream *istream;
    uint8 *read_buffer;
    uint32 read_buffer_pointer;
    uint32 read_buffer_size;
    Symbol *token_buffer;
    uint32 token_buffer_pointer;
    uint32 token_buffer_size;
    uint32 stashed_symbol;

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

    Symbol get_next_symbol();

    Symbol read_next_symbol();

    void stash_symbol(Symbol symbol);

    SSize update_buffer();

};

} //namespace psh

#endif //PHANTOMSHELL_LEXER_H
