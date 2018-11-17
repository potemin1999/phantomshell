/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/7/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_STRING_H
#define PHANTOMSHELL_STRING_H

#include "alloc.h"

#define STRING_VALUE_TERMINATOR 1

namespace phlib {

typedef unsigned int string_length;

/**
 * @class string is wrap class for char*
 */
class string {

private:
    char *str_value = nullptr;
    string_length str_length = 0;

public:

    /**
     * @brief Creates string by copying other string value
     * @param other
     */
    string(const string &other) {
        setup_string(this, other.str_value, other.str_length);
    }

    /**
     * @brief Creates string from null terminated char*
     * @param str
     */
    string(const char *str) {
        setup_string(this, str);
    }

    /**
     * @brief Creates empty string
     */
    string() = default;

    /**
     * @brief Destroys string and its resources
     */
    ~string() {
        if (str_value != nullptr) {
            phlib::free(str_value);
        }
    }

    /**
     * @brief assignment operator
     * @param other second operand
     * @return this with data of other
     */
    string &operator=(const string &other) = default;

    /**
     * @brief assignment operator
     * @param str second operand
     * @return this with str value
     */
    string &operator=(const char *&str) {
        setup_string(this, str);
        return *this;
    }

    /**
     * @brief += operator
     * @param other string
     * @return this string with appended @p other
     */
    string &operator+=(const string &other) {
        return plus_equal_operator(other.str_value, other.str_length);
    }

    /**
     * @brief += operator
     * @param str value to append
     * @return this string with appended @p str
     */
    string &operator+=(const char *str) {
        string_length length = 0;
        while (str[length++] != '\0');
        return plus_equal_operator(str, length);
    }

    /**
     * @brief allocates memory for new string
     * @param size of string
     * @return pointer to allocated memory
     */
    void *operator new(unsigned long size) {
        void *pointer = phlib::malloc((size + STRING_VALUE_TERMINATOR) * sizeof(char));
        return pointer;
    }

    /**
     * @brief releases memory allocated for string at @p pointer
     * @param pointer to string
     */
    void operator delete(void *pointer) {
        phlib::free(pointer);
    }

    /**
     * @brief Length of the string value
     * @return length of the string without null terminator
     */
    inline string_length length() {
        return str_length;
    }

    /**
     * @brief <tt>char*</tt> value of string
     * @return null terminated <tt>char*</tt>
     */
    inline const char *value() {
        return str_value;
    }

    /**
     * @brief Compares beginning of the string with other string value
     * @param str other string
     * @return true, if this string starts with @p str
     */
    inline bool starts_with(const string &str) {
        return starts_with(str.str_value);
    }

    /**
     * @brief Compares beginning of the string with other char* value
     * @param prefix char* to compare beginning with
     * @return true, if string starts with prefix
     */
    inline bool starts_with(const char *prefix) {
        for (int i = 0; prefix[i] != '\0'; i++) {
            if (i >= this->str_length) return false;
            if (prefix[i] != this->str_value[i]) return false;
        }
        return true;
    }

    /**
     * @brief Checks, whether this string ends by other string
     * @param suffix string to check
     * @return true if ends, false if does not
     */
    inline bool ends_with(const string &suffix) {
        return ends_with(suffix.str_value, suffix.str_length);
    }

    /**
     * @brief Checks, whether string ends with suffix
     * @param suffix to check
     * @return true if ends, false if does not
     */
    inline bool ends_with(const char *suffix) {
        string_length length = 0;
        while (suffix[++length] != '\0');
        return ends_with(suffix, length);
    }

    /**
     * @brief Checks, whether string ends with suffix
     *
     * If you know suffix length, this method is better than ends_with(const char*)
     * @param suffix - this is (probably) ending of this string
     * @param suffix_length suffix length without null terminator
     * @return true if ends, false if does not
     */
    inline bool ends_with(const char *suffix, string_length suffix_length) {
        if (suffix_length > this->str_length) return false;
        for (int i = 0; i <= suffix_length; i++) {
            if (this->str_value[i + this->str_length - suffix_length] != suffix[i]) return false;
        }
        return true;
    }

    /**
     * @brief Computes hash code of string by Java-like method
     * @return hash code of the string
     */
    unsigned int hash_code() {
        unsigned int hash = 0;
        if (str_length > 0) {
            for (int i = 0; i < str_length; i++) {
                hash = (hash << 5) - hash + str_value[i];
            }
        }
        return hash;
    }

    /**
     * @brief Compares two strings
     * @param other second string
     * @return true, if they have the same value
     */
    inline bool equals(const string &other) {
        return equals(other.str_value);
    }

    /**
     * @brief Compares string and char* values
     * @param str second operand
     * @return true, if value of string equals to str
     */
    inline bool equals(const char *str) {
        int i = 0;
        for (; str[i] != '\0'; i++) {
            if (str[i] != this->str_value[i]) return false;
        }
        return i == this->str_length;
    }

private:

    inline string &plus_equal_operator(const char *str, string_length str_length) {
        string_length length = str_length + this->str_length;
        string_length old_length = this->str_length;
        const char *last_value = this->str_value;
        this->str_value = (char *) phlib::malloc(length * sizeof(char) + STRING_VALUE_TERMINATOR);
        add_str(str_value, last_value, old_length, str, str_length);
#if STRING_VALUE_TERMINATOR == 1
        this->str_value[length] = '\0';
#endif //STRING_VALUE_TERMINATOR
        phlib::free((void *) last_value);
        this->str_length = length;
        return *this;
    }

    inline static void setup_string(string *dst, const char *src) {
        dst->str_length = 0;
        for (; src[dst->str_length] != '\0'; dst->str_length++);
        setup_string(dst, src, dst->str_length);
    }

    inline static void setup_string(string *dst, const char *src, string_length length) {
        dst->str_length = length;
        dst->str_value = (char *) malloc(dst->str_length + STRING_VALUE_TERMINATOR);
        add_str(dst->str_value, src, dst->str_length, nullptr, 0);
#if STRING_VALUE_TERMINATOR == 1
        dst->str_value[dst->str_length] = '\0';
#endif //STRING_VALUE_TERMINATOR
    }

    inline static void copy_str(char *dst, const char *src, int length) {
        for (int i = 0; i < length; dst[i] = src[i], i++);
    }

    inline static void add_str(char *dst, const char *src1, int src1_length, const char *src2, int src2_length) {
        copy_str(dst, src1, src1_length);
        copy_str(dst + src1_length, src2, src2_length);
    }

};

} //namespace phlib
#endif //PHANTOMSHELL_STRING_H