/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/3/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "types.h"

bool_to_str_func __bool_to_str = 0;
int_to_str_func __int_to_str = 0;
float_to_str_func __float_to_str = 0;
char_to_str_func __char_to_str = 0;

string_t convert_bool_to_string(bool_t) {

}
