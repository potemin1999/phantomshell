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

#include "Alloc.h"
#include "Character.h"
#include "IStream.h"
#include "Token.h"

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
    typedef Char16 Symbol;


private:
    //@formatter:off
    phlib::IStream *istream;             /**< data source */

    UInt8  *readBuffer;         /**< buffer as read source */
    UInt32 readBufferPointer;   /**< current position in read_buffer*/
    UInt32 readBufferSize;      /**< read buffer size in bytes */
    Symbol *stashBuffer;        /**< stash stack for symbols */
    UInt32 stashBufferPointer;  /**< stores token_buffer offset where the next symbol is stored */
    UInt32 stashBufferSize;     /**< max stash size */
    Int32  currentLine;         /**< variable stores current line, processing by lexer */

public:
    //@formatter:on
    /**
     * @brief Main constructor
     * @param inputStream stream from which lexer should lex
     */
    explicit Lexer(phlib::IStream *inputStream);

    /**
     * @brief Just destructor
     */
    ~Lexer();

    /**
     * @brief Operator new for creating lexer on stack
     * @param size is a size of lexer in memory
     * @return pointer to allocated memory
     */
    void *operator new(Size size) {
        return phlib::malloc(size);
    }

    /**
     * @brief Removes lexer from memory
     * @param ptr to lexer location
     */
    void operator delete(Ptr ptr) {
        phlib::free(ptr);
    }

    /**
     * @brief Returns next lexed token
     * @return token from input stream, nullptr if stream have ended
     */
    Token *getNextToken();

private:

    /**
     * @brief Get available symbol from input stream or from stash buffer
     * @return symbol if has one, '\0' otherwise
     */
    Symbol getNextSymbol();

    /**
     * @brief Reads and returns 1 symbol from input stream
     * @return symbol, huh
     */
    Symbol readNextSymbol();

    /**
     * @brief Trying to make operator token using @p char1 ad starting symbol
     * @param char1 is 1 symbol of operator (if it is going to be operator)
     * @return Token with operator type, if attempt to make was succeed, nullptr otherwise
     */
    Token *makeOperatorToken(Symbol char1);

    /**
     * @brief Main part of implementation Lexer::make_operator_token()
     * @param symbol is a first symbol of operator
     * @return psh::Operator or 0
     */
    Operator makeOperator(Symbol symbol);

    /**
     * @brief Try to make a separator from this symbol
     * @param symbol is 1 (and last, at now) symbol of separator
     * @return psh::Separator or 0
     */
    Separator makeSeparator(Symbol symbol);

    /**
     * @brief Puts symbol in stash buffer and updates corresponding variables
     * @param symbol to stash
     */
    void stashSymbol(Symbol symbol);

    /**
     * @brief Read new portion of symbols into internal buffer of psh::Lexer
     * @return read amount of read symbols
     */
    SSize updateBuffer();

    /**
     * @brief Try to make operator from identifier (and, or, xor, e.t.c.)
     * @return psh::Operator or 0;
     */
    Operator checkIfIdentifierIsOperator();

    /**
     * @brief Moves all content of stash buffer into string and empty stash buffer
     * @return stash buffer as phlib::String
     */
    phlib::String *createFromStashBuffer();

};

} //namespace psh

#endif //PHANTOMSHELL_LEXER_H
