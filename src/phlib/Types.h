/**
 * @headerfile
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/14/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_TYPES_H
#define PHANTOMSHELL_TYPES_H

#define WORDSIZE 4

#ifdef __simbuild__
#ifdef __unix__

#include <sys/types.h>

#endif //__unix__
#ifdef __WIN32

#include <cstdint>

typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;

#endif //__WIN32
#else //__simbuild__
#include <phantom_types.h>
#endif //__simbuild__

#define PHLIB_NAMESPACE_BEGIN namespace phlib{
#define PHLIB_NAMESPACE_END }

/**
 * byte types
 */
typedef signed char   byte; /**< [7]sign, [6:0] value,*/
typedef unsigned char ubyte; /**< [7:0] value */

/**
 * integer types
 */
typedef byte      int8; /**< same as byte */
typedef ubyte     uint8; /**< same as ubyte */
typedef int16_t   int16; /**< [15] sign, [14:0] value */
typedef u_int16_t uint16; /**< [15:0] value */
typedef int32_t   int32; /**< [31] sign, [30:0] value */
typedef u_int32_t uint32; /**< [31:0] value */
typedef int64_t   int64; /**< [63] sign, [62:0] value */
typedef u_int64_t uint64; /** [63:0] value */

/**
 * symbol types
 */
typedef signed char   char8; /** [7] sign, [6:0] value */
typedef unsigned char uchar8; /** [7:0] value */
typedef uint16        char16; /** type with 2 byte width for UTF-16 encoding */
typedef uint32        char32; /** type with 4 byte width for UTF-32 encoding */

/**
 * Word
 */
#if WORDSIZE == 2
typedef uint16 Word;
#else //WORDSIZE == 2
#if WORDSIZE == 4
typedef uint32 Word;
#else //WORDSIZE == 4
typedef uint64 Word;
#endif //WORDSIZE == 4
#endif //WORDSIZE == 2
/**
 * other types
 */
typedef void       *Ptr;
typedef const void *ConstPtr;
typedef byte       *BytePtr;
typedef const byte *ConstBytePtr;
typedef Word       *WordPtr;
typedef const Word *ConstWordPtr;

typedef size_t  Size;
typedef ssize_t SSize;

#endif //PHANTOMSHELL_TYPES_H
