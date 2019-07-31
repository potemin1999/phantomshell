/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/18/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_KEYWORD_H
#define PHANTOMSHELL_KEYWORD_H

#include "String.h"
#include "Types.h"

/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief Describes keywords of Phantom Shell
 */
typedef enum Keyword {
    //@formatter:off
    DEF    = 0x00, /**< def */
    RETURN = 0x01, /**< return */
    IF     = 0x10, /**< if */
    ELIF   = 0x11, /**< elif */
    ELSE   = 0x12, /**< else */
    FOR    = 0x13, /**< for */
    WHILE  = 0x14, /**< while */
    DO     = 0x15, /**< do */
    SWITCH = 0x16, /**< switch */
    CASE   = 0x17, /**< case */
    OTHER  = 0x18, /**< other */
    CLASS  = 0x20 /**< class */
    //@formatter:on
} Keyword;

/**
 * @brief checks, if word is a keyword
 * @param str
 * @return psh::int32 if succeed
 *         -1 otherwise
 */
inline Int32 IsKeyword(phlib::String &str) {
    switch (str[0]) {
        case 'c' : {
            if (str.equals("case")) return psh::Keyword::CASE;
            if (str.equals("class")) return psh::Keyword::CLASS;
            return -1;
        }
        case 'd' : {
            if (str.equals("def")) return psh::Keyword::DEF;
            if (str.equals("do")) return psh::Keyword::DO;
            return -1;
        }
        case 'i' : {
            if (str.equals("if")) return psh::Keyword::IF;
            return -1;
        }
        case 'e' : {
            if (str.equals("elif")) return psh::Keyword::ELIF;
            if (str.equals("else")) return psh::Keyword::ELSE;
            return -1;
        }
        case 'f' : {
            if (str.equals("for")) return psh::Keyword::FOR;
            return -1;
        }
        case 'o' : {
            if (str.equals("other")) return psh::Keyword::OTHER;
            return -1;
        }
        case 'r' : {
            if (str.equals("return")) return psh::Keyword::RETURN;
            return -1;
        }
        case 's' : {
            if (str.equals("switch")) return psh::Keyword::SWITCH;
            return -1;
        }
        case 'w' : {
            if (str.equals("while")) return psh::Keyword::WHILE;
            return -1;
        }
        default:return -1;
    }
}

inline const char *KeywordToString(psh::Keyword keyword) {
    switch (keyword) {
        case psh::Keyword::CASE: return "CASE";
        case psh::Keyword::CLASS: return "CLASS";
        case psh::Keyword::DEF: return "DEF";
        case psh::Keyword::DO: return "DO";
        case psh::Keyword::IF: return "IF";
        case psh::Keyword::ELIF: return "ELIF";
        case psh::Keyword::FOR: return "FOR";
        case psh::Keyword::OTHER: return "OTHER";
        case psh::Keyword::RETURN: return "RETURN";
        case psh::Keyword::SWITCH: return "SWITCH";
        case psh::Keyword::WHILE: return "WHILE";
        default: return "??_KEYWORD";
    }
}

} //namespace psh
#endif //PHANTOMSHELL_KEYWORD_H
