/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 7/31/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_TYPES_H
#define SHELL_TYPES_H

#include <stdint-gcc.h>
#include <uchar.h>

#define TYPE_UNKNWN 0b0000
#define TYPE_NONE   0b1111
#define TYPE_BOOL   0b0010
#define TYPE_INT    0b0100
#define TYPE_FLOAT  0b0110
#define TYPE_CHAR   0b1000
#define TYPE_STRING 0b1010
#define TYPE_ARRAY  0b1110
#define TYPE_OBJ    0b0001

//@formatter:off
typedef unsigned char   ubyte_t;
typedef unsigned char   bool_t;
typedef int32_t         int_t;
typedef float           float_t;
typedef char16_t        char_t;
typedef char*           string_t;
//@formatter:on

#define EXPR_NODE_TYPE(node) (((ast_node_expr_t*) node)->flags & 0x0Fu)

#endif //SHELL_TYPES_H
