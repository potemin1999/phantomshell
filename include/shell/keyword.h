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

/** @brief Default Shell namespace */
namespace psh {

/**
 * @brief Describes keywords of Phantom Shell
 */
typedef enum Keyword {
    DEF      = 0x00, /**< def */
    RETURN   = 0x01, /**< return */
    IF       = 0x10, /**< if */
    ELIF     = 0x11, /**< elif */
    ELSE     = 0x12, /**< else */
    FOR      = 0x13, /**< for */
    WHILE    = 0x14, /**< while */
    DO       = 0x15, /**< do */
    SWITCH   = 0x16, /**< switch */
    CASE     = 0x17, /**< case */
    OTHER    = 0x18, /**< other */
    CLASS    = 0x20 /**< class */
} Keyword;

} //namespace psh
#endif //PHANTOMSHELL_KEYWORD_H
