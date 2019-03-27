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
    virtual const phlib::String getTypeName(){
        return phlib::String("BaseType");
    };
};

class PshBoolean : PshType {
public:

    uint8 value;

    PshBoolean(bool value) {
        this->value = (uint8) (value ? 1 : 0);
    }

    PshBoolean(const PshBoolean &other) {
        this->value = other.value;
    }

    const phlib::String getTypeName() override{
        return phlib::String("Boolean");
    }

};

class PshInteger : PshType {
public:

    int32 value;

    PshInteger(int value) {
        this->value = (int32) value;
    }

    PshInteger(const PshInteger &other) {
        this->value = other.value;
    }

    const phlib::String getTypeName() override{
        return phlib::String("Integer");
    }

};

class PshFloat : PshType {
public:

    union {
        float value;
        struct {
            uint8  sign: 1;
            uint8  exponent: 8;
            uint32 fraction: 23;
        };
    };

    PshFloat(float value) {
        this->value = value;
    }

    const phlib::String getTypeName() override{
        return phlib::String("Float");
    }
};

class PshCharacter : PshType {
public:

    uint16 value;

    PshCharacter(char value) {
        this->value = (uint16) (0x00ff & value);
    }

    const phlib::String getTypeName() override{
        return phlib::String("Character");
    }

};

class PshString : PshType {
public:

    const char16 *value = nullptr;

    PshString(phlib::String &value) {
        this->value = value.value();
    }

    const phlib::String getTypeName() override{
        return phlib::String("String");
    }
};

class PshObject : PshType {

    const phlib::String getTypeName() override{
        return phlib::String("Object");
    }

};

} //namespace psh

#endif //PHANTOMSHELL_SHELL_TYPES_H
