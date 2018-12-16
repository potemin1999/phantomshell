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


phlib::Allocator *string_allocator = phlib::Allocator::get_default_allocator();

char16 phlib::String::digits[] = {'0', '1', '2', '3', '4',
                                  '5', '6', '7', '8', '9'};

int phlib::String::int_size_table[] = {9, 99, 999, 9999, 99999,
                                       999999, 9999999,
                                       99999999, 999999999};

phlib::String::String(const String &other) {
    str_value  = (char16 *) phlib::malloc((other.str_length + 1) * sizeof(char16));
    str_length = other.str_length;
    copy_str(str_value, other.str_value, other.str_length + 1);
}


phlib::String::String(const char16 *str, Size length) {
    str_length = length;
    str_value  = (char16 *) phlib::malloc((str_length + 1) * sizeof(char16));
    copy_str(str_value, str, str_length);
    str_value[str_length] = '\0';
}


phlib::String::String(const char *str) {
    str_length = get_length(str);
    str_value  = (char16 *) malloc((str_length + 1) * 2);
    for (int i            = 0; i < str_length; i++) {
        str_value[i] = str[i] & ((char16) 0x007f);
    }
    str_value[str_length] = '\0';
}


phlib::String::String() {
    str_length = 0;
    str_value  = (char16 *) malloc(1 * sizeof(char16));
    str_value[0] = '\0';
}


phlib::String::~String() {
    if (str_value != nullptr) {
        phlib::free(str_value);
        str_value = nullptr;
    }
    if (str_char_value != nullptr) {
        phlib::free(str_char_value);
        str_char_value = nullptr;
    }
}


phlib::String &phlib::String::operator=(const phlib::String &other) {
    this->str_value = (char16 *) malloc((other.str_length + 1) * 2);
    copy_str(str_value, other.str_value, other.str_length + 1);
    this->str_length = other.str_length;
}


phlib::String &phlib::String::operator=(const char16 *&str) {
    setup_string(this, str);
    return *this;
}


phlib::String &phlib::String::operator+=(const phlib::String &other) {
    return plus_equal_operator(other.str_value, other.str_length);
}


phlib::String &phlib::String::operator+=(const char16 *str) {
    Size length = 0;
    while (str[length++] != '\0');
    return plus_equal_operator(str, length);
}


Ptr phlib::String::operator new(unsigned long size) {
    return string_allocator->allocate(size);
}


void phlib::String::operator delete(Ptr pointer) {
    string_allocator->deallocate(pointer);
}


const char *phlib::String::char_value() {
    if (this->str_char_value == nullptr) {
        this->str_char_value = (char *) phlib::malloc(this->str_length + 1);
        for (int i                             = 0; i < this->str_length; i++) {
            this->str_char_value[i] = (char) (0x7f & this->str_value[i]);
        }
        this->str_char_value[this->str_length] = '\0';
    }
    return this->str_char_value;
}


bool phlib::String::starts_with(const phlib::String &str) {
    return starts_with(str.str_value);
}


bool phlib::String::starts_with(const char16 *prefix) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (i >= this->str_length) return false;
        if (prefix[i] != this->str_value[i]) return false;
    }
    return true;
}


bool phlib::String::ends_with(const String &suffix) {
    return ends_with(suffix.str_value, suffix.str_length);
}


bool phlib::String::ends_with(const char16 *suffix) {
    Size length = 0;
    while (suffix[++length] != '\0');
    return ends_with(suffix, length);
}


bool phlib::String::ends_with(const char16 *suffix, Size suffix_length) {
    if (suffix_length > this->str_length) return false;
    for (int i = 0; i <= suffix_length; i++) {
        if (this->str_value[i + this->str_length - suffix_length] != suffix[i]) return false;
    }
    return true;
}


bool phlib::String::equals(const String &other) {
    return equals(other.str_value);
}


bool phlib::String::equals(const char16 *str) {
    int i = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] != this->str_value[i]) return false;
    }
    return i == this->str_length;
}


int phlib::String::compare(const String &str1, const String &str2) {
    if (str1.str_length != str2.str_length)
        return str1.str_length > str2.str_length ? 1 : -1;
    return compare(str1.str_value, str2.str_value);
}


int phlib::String::compare(const char16 *str1, const char16 *str2) {
    if (str1 == nullptr || str2 == nullptr) return -2;
    for (int i = 0;; i++) {
        if (str1[i] == '\0' & str2[i] == '\0') break;
        if (str1[i] != str2[i]) return str1[i] > str2[i] ? 1 : -1;
    }
    return 0;
}


uint32 phlib::String::hash_code() {
    uint32 hash = 0;
    if (str_length > 0) {
        for (int i = 0; i < str_length; i++) {
            hash = (hash << 5) - hash + str_value[i];
        }
    }
    return hash;
}


phlib::String &phlib::String::plus_equal_operator(const char16 *str, Size str_length) {
    Size   length      = str_length + this->str_length;
    Size   old_length  = this->str_length;
    char16 *last_value = this->str_value;
    this->str_value = (char16 *) phlib::malloc((length + 1) * sizeof(char16));
    add_str(str_value, last_value, old_length, str, str_length);
    this->str_value[length] = '\0';
    if (last_value != nullptr) {
        phlib::free((void *) last_value);
    }
    this->str_length = length;
    return *this;
}


Size phlib::String::get_length(const char *str) {
    Size i = 0;
    for (; str[i] != '\0'; ++i);
    return i;
}


Size phlib::String::get_length(const char16 *str) {
    Size i = 0;
    for (; str[i] != '\0'; ++i);
    return i;
}


void phlib::String::setup_string(phlib::String *dst, const char16 *src) {
    dst->str_length = 0;
    for (; src[dst->str_length] != '\0'; dst->str_length++);
    setup_string(dst, src, dst->str_length);
}


void phlib::String::setup_string(phlib::String *dst, const char *src) {
    dst->str_length = 0;
    for (; src[dst->str_length] != '\0'; dst->str_length++);
    setup_string(dst, src, dst->str_length);
}


void phlib::String::setup_string(phlib::String *dst, const char *src, Size length) {
    dst->str_length = length;
    dst->str_value  = (char16 *) malloc((dst->str_length + 1) * 2);
    for (int i                      = 0; i < dst->str_length; i++) {
        dst->str_value[i] = src[i] & ((char16) 0x007f);
    }
    dst->str_value[dst->str_length] = '\0';
}


void phlib::String::setup_string(phlib::String *dst, const char16 *src, Size length) {
    dst->str_length = length;
    dst->str_value  = (char16 *) malloc((dst->str_length + 1) * 2);
    copy_str(dst->str_value, src, dst->str_length);
    dst->str_value[dst->str_length] = '\0';
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

int phlib::String::string_size_of_integer(int32 integer) {
    for (int i = 0;; i++)
        if (integer <= String::int_size_table[i])
            return i + 1;
}

phlib::String phlib::String::value_of(int32 value) {
    bool negative = value < 0;
    int  size     = string_size_of_integer(value);
    if (negative)
        size += 1;
    char16 chars[size];
    int    index  = size;
    int    q      = 0, r = 0;
    while (value != 0) {
        q     = value / 10;
        r     = value - (q * 10);
        value = q;
        chars[--index] = String::digits[r];
    }
    if (negative)
        chars[0]  = '-';
    return String(chars, size);
}
