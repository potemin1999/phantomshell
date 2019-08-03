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

//@formatter:off
typedef unsigned char   bool_t;
typedef int32_t         int_t;
typedef float           float_t;
typedef char16_t        char_t;
typedef char*           string_t;
//@formatter:on

typedef string_t (*bool_to_str_func)(bool_t val);

typedef string_t (*int_to_str_func)(int_t val);

typedef string_t (*float_to_str_func)(float_t val);

typedef string_t (*char_to_str_func)(char_t val);

extern bool_to_str_func __bool_to_str;
extern int_to_str_func __int_to_str;
extern float_to_str_func __float_to_str;
extern char_to_str_func __char_to_str;

#define BOOL_TO_STRING(bool_val, string_val_p)                          \
    if (__bool_to_str){*string_val_p = __bool_to_str(bool_val);         \
    }else{*string_val_p = strdup(__FILE__":"__LINE__"include types.c");}

#define INT_TO_STRING(int_val, string_val_p)    \
    if (__int_to_str)                           \
        *string_val_p = __int_to_str(int_val);  \
    else                                        \
        *string_val_p = strdup(__FILE__":"__LINE__" include types.c");


#endif //SHELL_TYPES_H
