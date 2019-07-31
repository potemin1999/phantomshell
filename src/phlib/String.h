/**
 * @headerfile
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/7/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_STRING_H
#define PHANTOMSHELL_STRING_H

#include "Alloc.h"
#include "Types.h"

/** @brief Default namespace for Phantom Shell support library */
namespace phlib {

/**
 * @brief String is wrap class for char16*
 */
class String {

private:
    //@formatter:off
    char          *strCharValue = nullptr;
    Char16        *strValue     = nullptr; /**< null terminated char16 sequence */
    Size          strLength     = 0;
    static Char16 digits[10];
    static int    intSizeTable[9];

public:
    //@formatter:on
    /**
     * @brief Creates string by copying other string value
     * @param other
     */
    String(const String &other);

    /**
     * @brief Creates string from null terminated char16 array
     * @param str is array
     */
    String(const Char16 *str) : String(str, getLength(str)) {}

    /**
     * @brief Creates string from char16* sequence with requested length
     * @param str is a pointer to string beginning
     * @param length is size of array to copy
     */
    String(const Char16 *str, Size length);

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
    Char16 &operator[](int i) {
        return strValue[i];
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
    String &operator=(const Char16 *&str);

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
    String &operator+=(const Char16 *str);

    /**
     * @brief allocates memory for new string
     * @param size of string
     * @return pointer to allocated memory
     */
    void *operator new(Size size);

    /**
     * @brief releases memory allocated for string at @p pointer
     * @param pointer to string
     */
    void operator delete(Ptr pointer);

    /**
     * @brief converts string to const char16 array
     * @return pointer to value;
     */
    inline operator const Char16 *() {
        return value();
    }

    /**
     * @brief converts string to const char array
     * @return pointer to char value
     */
    inline operator const char *() {
        return charValue();
    }

    /**
     * @brief Length of the string value
     * @return length of the string without null terminator
     */
    inline Size length() {
        return strLength;
    }

    /**
     * @brief <tt>char*</tt> value of string
     * @return null terminated <tt>char*</tt>
     */
    inline const Char16 *value() {
        return strValue;
    }

    /**
     * @brief default char value of string
     * @return null terminated const char array
     */
    const char *charValue();

    /**
     * @brief Compares beginning of the string with other string value
     * @param str other string
     * @return true, if this string starts with @p str
     */
    bool startsWith(const String &str);

    /**
     * @brief Compares beginning of the string with other char* value
     * @param prefix char* to compare beginning with
     * @return true, if string starts with prefix
     */
    bool startsWith(const Char16 *prefix);

    /**
     * @brief Checks, whether this string ends by other string
     * @param suffix string to check
     * @return true if ends, false if does not
     */
    bool endsWith(const String &suffix);

    /**
     * @brief Checks, whether string ends with suffix
     * @param suffix to check
     * @return true if ends, false if does not
     */
    bool endsWith(const Char16 *suffix);

    /**
     * @brief Checks, whether string ends with suffix
     *
     * If you know suffix length, this method is better than ends_with(const char*)
     * @param suffix - this is (probably) ending of this string
     * @param suffixLength suffix length without null terminator
     * @return true if ends, false if does not
     */
    bool endsWith(const Char16 *suffix, Size suffixLength);

    /**
     * @brief Computes hash code of string by Java-like method
     * @return hash code of the string
     */
    UInt32 hashCode();

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
    bool equals(const Char16 *str);

    int compare(const String &str1, const String &str2);

    int compare(const Char16 *str1, const Char16 *str2);

    static String valueOf(UInt32 value);

    static String valueOf(Int32 value);

    static String valueOf(UInt8 value) {
        return String(); //TODO: valueOf
    }

    static int strcmp(const char *str1, const char *str2);

private:

    static int stringSizeOfInteger(Int32 integer);

    String &plusEqualOperator(const Char16 *str, Size strLength);

    static Size getLength(const char *str);

    static Size getLength(const Char16 *str);

    static void setupString(String *dst, const Char16 *src);

    static void setupString(String *dst, const char *src);

    static void setupString(String *dst, const char *src, Size length);

    static void setupString(String *dst, const Char16 *src, Size length);

    inline static void copyStr(Char16 *dst, const Char16 *src, Size length) {
        for (int i = 0; i < length; dst[i] = src[i], i++);
    }

    inline static void addStr(Char16 *dst,
                              const Char16 *src1, Size src1Length,
                              const Char16 *src2, Size src2Length) {
        copyStr(dst, src1, src1Length);
        copyStr(dst + src1Length, src2, src2Length);
    }

};

} //namespace phlib
#endif //PHANTOMSHELL_STRING_H
