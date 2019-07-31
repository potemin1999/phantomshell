/**
 * @headerfile
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/18/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_CHARACTER_H
#define PHANTOMSHELL_CHARACTER_H

#include "Types.h"

/** @brief Default namespace for Phantom Shell support library */
PHLIB_NAMESPACE_BEGIN

/**
 * @brief Character class is used for char/char16 manipulations
 */
class Character {
public:

    Char16 value;

    Character(const char &value) {
        this->value = (Char16) (0x007f & value);
    }

    Character(const Char16 &value) {
        this->value = value;
    }

    Character(int value) {
        this->value = (Char16) value;
    }

    operator char() {
        return (char) (this->value & 0x7f);
    }

    operator Char16() {
        return this->value;
    }

    bool operator==(const Char16 &c) {
        return this->value == c;
    }

    bool operator==(const char &c) {
        return ((char) this->value) == c;
    }

    inline bool isDigit() {
        return isDigit(this->value);
    }

    inline bool isLetter() {
        return isLetter(this->value);
    }

    static inline bool isDigit(Char16 symbol) {
        return symbol >= '0' & symbol <= '9';
    }

    static inline bool isLetter(Char16 symbol) {
        return (symbol > 0x40 & symbol < 0x5b) || (symbol > 0x60 & symbol < 0x7b);
    }

};

PHLIB_NAMESPACE_END

#endif //PHANTOMSHELL_CHARACTER_H
