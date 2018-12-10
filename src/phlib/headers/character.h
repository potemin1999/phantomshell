/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/18/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_CHARACTER_H
#define PHANTOMSHELL_CHARACTER_H

#include "types.h"

/** @brief Default namespace for Phantom Shell support library */
namespace phlib{

/**
 * @brief Character class is used for char/char16 manipulations
 */
class Character{
public:

    char16 value;

    Character(const char& value){
        this->value = (char16)(0x007f & value);
    }

    Character(const char16 &value){
        this->value = value;
    }

    Character(int value){
        this->value = (char16) value;
    }

    operator char(){
        return (char)(this->value & 0x7f);
    }

    operator char16(){
        return this->value;
    }

    bool operator ==(const char16& c){
        return this->value == c;
    }

    bool operator ==(const char& c){
        return ((char) this->value) == c;
    }

    inline bool is_digit(){
        return is_digit(this->value);
    }

    inline bool is_letter(){
        return is_letter(this->value);
    }

    static inline bool is_digit(char16 symbol){
        return symbol >= '0' & symbol <= '9';
    }

    static inline bool is_letter(char16 symbol){
        return (symbol > 0x40 & symbol < 0x5b) || (symbol > 0x60 & symbol < 0x7b);
    }

};

}

#endif //PHANTOMSHELL_CHARACTER_H
