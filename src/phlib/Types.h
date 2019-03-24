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

//@formatter:off

/**
 * byte types
 */
typedef signed char   Byte; /**< [7]sign, [6:0] value,*/
typedef unsigned char UByte; /**< [7:0] value */

/**
 * integer types
 */
typedef Byte      Int8; /**< same as byte */
typedef UByte     UInt8; /**< same as ubyte */
typedef int16_t   Int16; /**< [15] sign, [14:0] value */
typedef u_int16_t UInt16; /**< [15:0] value */
typedef int32_t   Int32; /**< [31] sign, [30:0] value */
typedef u_int32_t UInt32; /**< [31:0] value */
typedef int64_t   Int64; /**< [63] sign, [62:0] value */
typedef u_int64_t UInt64; /** [63:0] value */

/**
 * symbol types
 */
typedef signed char   Char8; /** [7] sign, [6:0] value */
typedef unsigned char UChar8; /** [7:0] value */
typedef UInt16        Char16; /** type with 2 byte width for UTF-16 encoding */
typedef UInt32        Char32; /** type with 4 byte width for UTF-32 encoding */

/**
 * Word
 */
#if WORDSIZE == 2
typedef UInt16 Word;
#else //WORDSIZE == 2
#if WORDSIZE == 4
typedef UInt32 Word;
#else //WORDSIZE == 4
typedef UInt64 Word;
#endif //WORDSIZE == 4
#endif //WORDSIZE == 2
/**
 * other types
 */
typedef void       *Ptr;
typedef const void *ConstPtr;
typedef Byte       *BytePtr;
typedef const Byte *ConstBytePtr;
typedef Word       *WordPtr;
typedef const Word *ConstWordPtr;

typedef size_t  Size;
typedef ssize_t SSize;

//@formatter:on

#endif //PHANTOMSHELL_TYPES_H
