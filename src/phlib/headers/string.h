/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/7/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_STRING_H
#define PHANTOMSHELL_STRING_H

#include "alloc.h"
#include "types.h"

/** @brief Default namespace for Phantom Shell support library */
namespace phlib {

/**
 * @brief String is wrap class for char16*
 */
class String {

private:

    char          *str_char_value = nullptr;
    char16        *str_value      = nullptr; /**< null terminated char16 sequence */
    Size          str_length      = 0;
    static char16 digits[10];
    static int    int_size_table[9];

public:

    /**
     * @brief Creates string by copying other string value
     * @param other
     */
    String(const String &other);

    /**
     * @brief Creates string from null terminated char16 array
     * @param str is array
     */
    String(const char16 *str) : String(str, get_length(str)) {}

    /**
     * @brief Creates string from char16* sequence with requested length
     * @param str is a pointer to string beginning
     * @param length is size of array to copy
     */
    String(const char16 *str, Size length);

    /**
     * @brief Creates string from null terminated char array
     * @param str is array
     */
    String(const char *str);

    /**
     * @brief Creates empty string
     */
    String();

    /**
     * @brief Destroys string and its resources
     */
    ~String();

    /**
     * @brief returns character at required index
     * @param i index of character
     * @return char16 value
     */
    char16 &operator[](int i) {
        return str_value[i];
    }

    /**
     * @brief assignment operator
     * @param other second operand
     * @return this with data of other
     */
    String &operator=(const String &other);

    /**
     * @brief assignment operator
     * @param str second operand
     * @return this with str value
     */
    String &operator=(const char16 *&str);

    /**
     * @brief += operator
     * @param other string
     * @return this string with appended @p other
     */
    String &operator+=(const String &other);

    /**
     * @brief += operator
     * @param str value to append
     * @return this string with appended @p str
     */
    String &operator+=(const char16 *str);

    /**
     * @brief allocates memory for new string
     * @param size of string
     * @return pointer to allocated memory
     */
    void *operator new(unsigned long size);

    /**
     * @brief releases memory allocated for string at @p pointer
     * @param pointer to string
     */
    void operator delete(void *pointer);

    /**
     * @brief converts string to const char16 array
     * @return pointer to value;
     */
    inline operator const char16 *() {
        return value();
    }

    /**
     * @brief converts string to const char array
     * @return pointer to char value
     */
    inline operator const char *() {
        return char_value();
    }

    /**
     * @brief Length of the string value
     * @return length of the string without null terminator
     */
    inline Size length() {
        return str_length;
    }

    /**
     * @brief <tt>char*</tt> value of string
     * @return null terminated <tt>char*</tt>
     */
    inline const char16 *value() {
        return str_value;
    }

    /**
     * @brief default char value of string
     * @return null terminated const char array
     */
    const char *char_value();

    /**
     * @brief Compares beginning of the string with other string value
     * @param str other string
     * @return true, if this string starts with @p str
     */
    bool starts_with(const String &str);

    /**
     * @brief Compares beginning of the string with other char* value
     * @param prefix char* to compare beginning with
     * @return true, if string starts with prefix
     */
    bool starts_with(const char16 *prefix);

    /**
     * @brief Checks, whether this string ends by other string
     * @param suffix string to check
     * @return true if ends, false if does not
     */
    bool ends_with(const String &suffix);

    /**
     * @brief Checks, whether string ends with suffix
     * @param suffix to check
     * @return true if ends, false if does not
     */
    bool ends_with(const char16 *suffix);

    /**
     * @brief Checks, whether string ends with suffix
     *
     * If you know suffix length, this method is better than ends_with(const char*)
     * @param suffix - this is (probably) ending of this string
     * @param suffix_length suffix length without null terminator
     * @return true if ends, false if does not
     */
    bool ends_with(const char16 *suffix, Size suffix_length);

    /**
     * @brief Computes hash code of string by Java-like method
     * @return hash code of the string
     */
    uint32 hash_code();

    /**
     * @brief Compares two strings
     * @param other second string
     * @return true, if they have the same value
     */
    bool equals(const String &other);

    /**
     * @brief Compares string and char* values
     * @param str second operand
     * @return true, if value of string equals to str
     */
    bool equals(const char16 *str);

    int compare(const String &str1, const String &str2);

    int compare(const char16 *str1, const char16 *str2);

    static String value_of(uint32 value) {

    }

    static String value_of(int32 value);

    static String value_of(uint8 value) {

    }

    static int strcmp(const char *str1, const char *str2);

private:

    static int string_size_of_integer(int32 integer);

    String &plus_equal_operator(const char16 *str, Size str_length);

    static Size get_length(const char *str);

    static Size get_length(const char16 *str);

    static void setup_string(String *dst, const char16 *src);

    static void setup_string(String *dst, const char *src);

    static void setup_string(String *dst, const char *src, Size length);

    static void setup_string(String *dst, const char16 *src, Size length);

    inline static void copy_str(char16 *dst, const char16 *src, Size length) {
        for (int i = 0; i < length; dst[i] = src[i], i++);
    }

    inline static void add_str(char16 *dst,
                               const char16 *src1, Size src1_length,
                               const char16 *src2, Size src2_length) {
        copy_str(dst, src1, src1_length);
        copy_str(dst + src1_length, src2, src2_length);
    }

};

} //namespace phlib
#endif //PHANTOMSHELL_STRING_H
