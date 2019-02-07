/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <string.h>
#include <character.h>

#include "string.h"


phlib::Allocator *string_allocator = phlib::Allocator::getDefaultAllocator();

char16 phlib::String::digits[] = {'0', '1', '2', '3', '4',
                                  '5', '6', '7', '8', '9'};

int phlib::String::intSizeTable[] = {9, 99, 999, 9999, 99999,
                                     999999, 9999999,
                                     99999999, 999999999};

phlib::String::String(const String &other) {
    strValue  = (char16 *) phlib::malloc((other.strLength + 1) * sizeof(char16));
    strLength = other.strLength;
    copyStr(strValue, other.strValue, other.strLength + 1);
}


phlib::String::String(const char16 *str, Size length) {
    strLength = length;
    strValue  = (char16 *) phlib::malloc((strLength + 1) * sizeof(char16));
    copyStr(strValue, str, strLength);
    strValue[strLength] = '\0';
}


phlib::String::String(const char *str) {
    strLength = getLength(str);
    strValue  = (char16 *) malloc((strLength + 1) * 2);
    for (int i          = 0; i < strLength; i++) {
        strValue[i] = str[i] & ((char16) 0x007f);
    }
    strValue[strLength] = '\0';
}


phlib::String::String() {
    strLength = 0;
    strValue  = (char16 *) malloc(1 * sizeof(char16));
    strValue[0] = '\0';
}


phlib::String::~String() {
    if (strValue != nullptr) {
        phlib::free(strValue);
        strValue = nullptr;
    }
    if (strCharValue != nullptr) {
        phlib::free(strCharValue);
        strCharValue = nullptr;
    }
}


phlib::String &phlib::String::operator=(const phlib::String &other) {
    this->strValue = (char16 *) malloc((other.strLength + 1) * 2);
    copyStr(strValue, other.strValue, other.strLength + 1);
    this->strLength = other.strLength;
}


phlib::String &phlib::String::operator=(const char16 *&str) {
    setupString(this, str);
    return *this;
}


phlib::String &phlib::String::operator+=(const phlib::String &other) {
    return plusEqualOperator(other.strValue, other.strLength);
}


phlib::String &phlib::String::operator+=(const char16 *str) {
    Size length = 0;
    while (str[length++] != '\0');
    return plusEqualOperator(str, length);
}


Ptr phlib::String::operator new(Size size) {
    return string_allocator->allocate(size);
}


void phlib::String::operator delete(Ptr pointer) {
    string_allocator->deallocate(pointer);
}


const char *phlib::String::charValue() {
    if (this->strCharValue == nullptr) {
        this->strCharValue = (char *) phlib::malloc(this->strLength + 1);
        for (int i = 0; i < this->strLength; i++) {
            this->strCharValue[i] = (char) (0x7f & this->strValue[i]);
        }

        this->strCharValue[this->strLength] = '\0';
    }
    return this->strCharValue;
}


bool phlib::String::startsWith(const phlib::String &str) {
    return startsWith(str.strValue);
}


bool phlib::String::startsWith(const char16 *prefix) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (i >= this->strLength) return false;
        if (prefix[i] != this->strValue[i]) return false;
    }
    return true;
}


bool phlib::String::endsWith(const String &suffix) {
    return endsWith(suffix.strValue, suffix.strLength);
}


bool phlib::String::endsWith(const char16 *suffix) {
    Size length = 0;
    while (suffix[++length] != '\0');
    return endsWith(suffix, length);
}


bool phlib::String::endsWith(const char16 *suffix, Size suffixLength) {
    if (suffixLength > this->strLength) return false;
    for (int i = 0; i <= suffixLength; i++) {
        if (this->strValue[i + this->strLength - suffixLength] != suffix[i]) return false;
    }
    return true;
}


bool phlib::String::equals(const String &other) {
    return equals(other.strValue);
}


bool phlib::String::equals(const char16 *str) {
    int i = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] != this->strValue[i]) return false;
    }
    return i == this->strLength;
}


int phlib::String::compare(const String &str1, const String &str2) {
    if (str1.strLength != str2.strLength)
        return str1.strLength > str2.strLength ? 1 : -1;
    return compare(str1.strValue, str2.strValue);
}


int phlib::String::compare(const char16 *str1, const char16 *str2) {
    if (str1 == nullptr || str2 == nullptr) return -2;
    for (int i = 0;; i++) {
        if (str1[i] == '\0' & str2[i] == '\0') break;
        if (str1[i] != str2[i]) return str1[i] > str2[i] ? 1 : -1;
    }
    return 0;
}


uint32 phlib::String::hashCode() {
    uint32 hash = 0;
    if (strLength > 0) {
        for (int i = 0; i < strLength; i++) {
            hash = (hash << 5) - hash + strValue[i];
        }
    }
    return hash;
}


phlib::String &phlib::String::plusEqualOperator(const char16 *str, Size strLength) {
    auto length    = strLength + this->strLength;
    auto oldLength = this->strLength;
    auto lastValue = this->strValue;
    this->strValue = (char16 *) phlib::malloc((length + 1) * sizeof(char16));
    addStr(strValue, lastValue, oldLength, str, strLength);
    this->strValue[length] = '\0';
    if (lastValue != nullptr) {
        phlib::free((void *) lastValue);
    }
    this->strLength = length;
    return *this;
}


Size phlib::String::getLength(const char *str) {
    Size i = 0;
    for (; str[i] != '\0'; ++i);
    return i;
}


Size phlib::String::getLength(const char16 *str) {
    Size i = 0;
    for (; str[i] != '\0'; ++i);
    return i;
}


void phlib::String::setupString(phlib::String *dst, const char16 *src) {
    dst->strLength = 0;
    for (; src[dst->strLength] != '\0'; dst->strLength++);
    setupString(dst, src, dst->strLength);
}


void phlib::String::setupString(phlib::String *dst, const char *src) {
    dst->strLength = 0;
    for (; src[dst->strLength] != '\0'; dst->strLength++);
    setupString(dst, src, dst->strLength);
}


void phlib::String::setupString(phlib::String *dst, const char *src, Size length) {
    dst->strLength = length;
    dst->strValue  = (char16 *) malloc((dst->strLength + 1) * 2);
    for (int i = 0; i < dst->strLength; i++) {
        dst->strValue[i] = src[i] & ((char16) 0x007f);
    }

    dst->strValue[dst->strLength] = '\0';
}


void phlib::String::setupString(phlib::String *dst, const char16 *src, Size length) {
    dst->strLength = length;
    dst->strValue  = (char16 *) malloc((dst->strLength + 1) * 2);
    copyStr(dst->strValue, src, dst->strLength);
    dst->strValue[dst->strLength] = '\0';
}

int phlib::String::strcmp(const char *str1, const char *str2) {
    for (int i = 0;; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] < str2[i] ? -1 : 1;
        }
        if (str1[i] == '\0') {
            return 0;
        }
    }
}

int phlib::String::stringSizeOfInteger(int32 integer) {
    for (int i = 0;; i++)
        if (integer <= String::intSizeTable[i])
            return i + 1;
}

phlib::String phlib::String::valueOf(int32 value) {
    bool negative = value < 0;
    auto size     = stringSizeOfInteger(value);
    if (negative)
        size += 1;
    char16 chars[size];
    auto   index  = size;
    auto   quot   = 0;
    auto   rmdr   = 0;
    while (value != 0) {
        quot  = value / 10;
        rmdr  = value - (quot * 10);
        value = quot;
        chars[--index] = String::digits[rmdr];
    }
    if (negative)
        chars[0]  = '-';
    return String(chars, size);
}
