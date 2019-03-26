/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "String.h"
#include "Character.h"

#include "String.h"

using namespace phlib;

Allocator *string_allocator = Allocator::getDefaultAllocator();

Char16 String::digits[] = {'0', '1', '2', '3', '4',
                           '5', '6', '7', '8', '9'};

int String::intSizeTable[] = {9, 99, 999, 9999, 99999,
                              999999, 9999999,
                              99999999, 999999999};

String::String(const String &other) {
    strValue = new Char16[other.strLength + 1];
    strLength = other.strLength;
    copyStr(strValue, other.strValue, other.strLength + 1);
}


String::String(const Char16 *str, Size length) {
    strLength = length;
    strValue = new Char16[strLength + 1];
    copyStr(strValue, str, strLength);
    strValue[strLength] = '\0';
}


String::String(const char *str) {
    strLength = getLength(str);
    strValue = new Char16[strLength + 1];
    for (int i = 0; i < strLength; i++) {
        strValue[i] = str[i] & (static_cast<Char16>(0x007f));
    }
    strValue[strLength] = '\0';
}


String::String() {
    strLength = 0;
    strValue = new Char16[1];
    strValue[0] = '\0';
}


String::~String() {
    if (strValue != nullptr) {
        delete[] strValue;
        strValue = nullptr;
    }
    if (strCharValue != nullptr) {
        delete[] strCharValue;
        strCharValue = nullptr;
    }
}


String &String::operator=(const String &other) {
    this->strValue = new Char16[other.strLength + 1];
    copyStr(strValue, other.strValue, other.strLength + 1);
    this->strLength = other.strLength;
}


String &String::operator=(const Char16 *&str) {
    setupString(this, str);
    return *this;
}


String &String::operator+=(const String &other) {
    return plusEqualOperator(other.strValue, other.strLength);
}


String &String::operator+=(const Char16 *str) {
    Size length = 0;
    while (str[length++] != '\0');
    return plusEqualOperator(str, length);
}


Ptr String::operator new(Size size) {
    return string_allocator->allocate(size);
}


void String::operator delete(Ptr pointer) {
    string_allocator->deallocate(pointer);
}


const char *String::charValue() {
    if (this->strCharValue == nullptr) {
        this->strCharValue = new char[this->strLength + 1];
        for (int i = 0; i < this->strLength; i++) {
            this->strCharValue[i] = (char) (0x7f & this->strValue[i]);
        }
        this->strCharValue[this->strLength] = '\0';
    }
    return this->strCharValue;
}


bool String::startsWith(const String &str) {
    return startsWith(str.strValue);
}


bool String::startsWith(const Char16 *prefix) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (i >= this->strLength) return false;
        if (prefix[i] != this->strValue[i]) return false;
    }
    return true;
}


bool String::endsWith(const String &suffix) {
    return endsWith(suffix.strValue, suffix.strLength);
}


bool String::endsWith(const Char16 *suffix) {
    Size length = 0;
    while (suffix[++length] != '\0');
    return endsWith(suffix, length);
}


bool String::endsWith(const Char16 *suffix, Size suffixLength) {
    if (suffixLength > this->strLength) return false;
    for (int i = 0; i <= suffixLength; i++) {
        if (this->strValue[i + this->strLength - suffixLength] != suffix[i]) return false;
    }
    return true;
}


bool String::equals(const String &other) {
    return equals(other.strValue);
}


bool String::equals(const Char16 *str) {
    int i = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] != this->strValue[i]) return false;
    }
    return i == this->strLength;
}


int String::compare(const String &str1, const String &str2) {
    if (str1.strLength != str2.strLength)
        return str1.strLength > str2.strLength ? 1 : -1;
    return compare(str1.strValue, str2.strValue);
}


int String::compare(const Char16 *str1, const Char16 *str2) {
    if (str1 == nullptr || str2 == nullptr) return -2;
    for (int i = 0;; i++) {
        if (str1[i] == '\0' & str2[i] == '\0') break;
        if (str1[i] != str2[i]) return str1[i] > str2[i] ? 1 : -1;
    }
    return 0;
}


UInt32 String::hashCode() {
    UInt32 hash = 0;
    if (strLength > 0) {
        for (int i = 0; i < strLength; i++) {
            hash = (hash << 5) - hash + strValue[i];
        }
    }
    return hash;
}


String &String::plusEqualOperator(const Char16 *str, Size strLength) {
    auto length = strLength + this->strLength;
    auto oldLength = this->strLength;
    auto lastValue = this->strValue;
    this->strValue = new Char16[length + 1];
    addStr(strValue, lastValue, oldLength, str, strLength);
    this->strValue[length] = '\0';
    delete[] lastValue;
    this->strLength = length;
    return *this;
}


Size String::getLength(const char *str) {
    Size i = 0;
    for (; str[i] != '\0'; ++i);
    return i;
}


Size String::getLength(const Char16 *str) {
    Size i = 0;
    for (; str[i] != '\0'; ++i);
    return i;
}


void String::setupString(String *dst, const Char16 *src) {
    dst->strLength = 0;
    for (; src[dst->strLength] != '\0'; dst->strLength++);
    setupString(dst, src, dst->strLength);
}


void String::setupString(String *dst, const char *src) {
    dst->strLength = 0;
    for (; src[dst->strLength] != '\0'; dst->strLength++);
    setupString(dst, src, dst->strLength);
}


void String::setupString(String *dst, const char *src, Size length) {
    dst->strLength = length;
    dst->strValue = new Char16[dst->strLength + 1];
    for (int i = 0; i < dst->strLength; i++) {
        dst->strValue[i] = src[i] & (static_cast<Char16>(0x007f));
    }

    dst->strValue[dst->strLength] = '\0';
}


void String::setupString(String *dst, const Char16 *src, Size length) {
    dst->strLength = length;
    dst->strValue = new Char16[dst->strLength + 1];
    copyStr(dst->strValue, src, dst->strLength);
    dst->strValue[dst->strLength] = '\0';
}

int String::strcmp(const char *str1, const char *str2) {
    for (int i = 0;; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] < str2[i] ? -1 : 1;
        }
        if (str1[i] == '\0') {
            return 0;
        }
    }
}

int String::stringSizeOfInteger(Int32 integer) {
    for (int i = 0;; i++)
        if (integer <= String::intSizeTable[i])
            return i + 1;
}

String String::valueOf(Int32 value) {
    bool negative = value < 0;
    auto size = stringSizeOfInteger(value);
    if (negative)
        size += 1;
    Char16 chars[size];
    auto index = size;
    auto quot = 0;
    auto rmdr = 0;
    while (value != 0) {
        quot = value / 10;
        rmdr = value - (quot * 10);
        value = quot;
        chars[--index] = String::digits[rmdr];
    }
    if (negative)
        chars[0] = '-';
    return String(chars, size);
}

String String::valueOf(UInt32 value) {
    return valueOf(static_cast<Int32>(value & 0x7fff));
}
