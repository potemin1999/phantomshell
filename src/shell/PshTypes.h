/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/15/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_SHELL_TYPES_H
#define PHANTOMSHELL_SHELL_TYPES_H

#include "String.h"
#include "Types.h"

/** @brief Default Shell namespace */
namespace psh {

class PshType {
public:

};

class PshBoolean : PshType {
public:

    UInt8 value;

    PshBoolean(bool value) {
        this->value = (UInt8) (value ? 1 : 0);
    }

    PshBoolean(const PshBoolean &other) {
        this->value = other.value;
    }

};

class PshInteger : PshType {
public:

    Int32 value;

    PshInteger(int value) {
        this->value = (Int32) value;
    }

    PshInteger(const PshInteger &other) {
        this->value = other.value;
    }

};

class PshFloat : PshType {
public:

    union {
        float value;
        struct {
            UInt8 sign: 1;
            UInt8 exponent: 8;
            UInt32 fraction: 23;
        };
    };

    PshFloat(float value) {
        this->value = value;
    }
};

class PshCharacter : PshType {
public:

    UInt16 value;

    PshCharacter(char value) {
        this->value = (UInt16) (0x00ff & value);
    }

};

class PshString : PshType {
public:

    const Char16 *value = nullptr;

    PshString(phlib::String &value) {
        this->value = value.value();
    }

};

class PshObject : PshType {

};

} //namespace psh

#endif //PHANTOMSHELL_SHELL_TYPES_H
