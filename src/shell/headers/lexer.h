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

#include "alloc.h"
#include "character.h"
#include "istream.h"
#include "token.h"

#define LEXER_READ_BUFFER_SIZE 8192
#define LEXER_STASH_BUFFER_SIZE 256

#ifdef __debug_lexer__
#define DEBUG_LOG_LEX(...) DEBUG_LOG(__VA_ARGS__)
#else
#define DEBUG_LOG_LEX(...)
#endif

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
    Symbol *stash_buffer;
    uint32 stash_buffer_pointer; /**< stores token_buffer offset where the next symbol is stored*/
    uint32 stash_buffer_size;

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

    uint32 check_if_identifier_is_operator();

    phlib::String *create_from_stash_buffer();

};

} //namespace psh

#endif //PHANTOMSHELL_LEXER_H
