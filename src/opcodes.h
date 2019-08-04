/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_OPCODES_H
#define SHELL_OPCODES_H

#include "operator.h"

typedef unsigned char opcode_t;

/** Does nothing */
#define OPCODE_NOP      0x00
#define OPCODE_EXSTK    0x10 // expands stack capacity
/** LOAD or SAVE
 * puts/loads something from/to local variable storage
 * [1]  0 - root scope
 *      1 - method scope
 * [7] index
 */
#define OPCODE_ILOAD    0x32 // ( -> value ) : loads integer
#define OPCODE_ISAVE    0x33 // ( value -> ) : saves integer
#define OPCODE_FLOAD    0x34 // ( value -> ) : loads float
#define OPCODE_FSAVE    0x35 // ( value -> ) : saves float
/**
 * CONST puts value on the stack
 * B = [1] byte
 * I = [4] bytes
 * F = [4] bytes
 */
#define OPCODE_ICONST   0x36 // ( -> value ) : puts integer on the stack
#define OPCODE_FCONST   0x37 // ( -> value ) : puts float on the stack
#define OPCODE_IADD     0x42 // ( value_1,value_2 -> value_3) : value_1 + value_2 = value_3, integers
#define OPCODE_ISUB     0x43 // ( value_1,value_2 -> value_3) : value_1 - value_2 = value_3, integers
#define OPCODE_IMUL     0x44 // ( value_1,value_2 -> value_3) : value_1 * value_2 = value_3, integers
#define OPCODE_IDIV     0x45 // ( value_1,value_2 -> value_3) : value_1 / value_2 = value_3, integers
#define OPCODE_PANIC    0xff // stops execution

const char *get_opcode_name(opcode_t opcode);

opcode_t int_operator_to_opcode(int oper);

#define OPCODE_SAVE(type_id, opcode_p)                      \
    switch (type_id){                                       \
        case TYPE_INT: { *opcode_p = OPCODE_ISAVE; break;}  \
        case TYPE_FLOAT: { *opcode_p = OPCODE_FSAVE; break;}\
        default: {                                          \
            printf("unable to save type %d",type_id);       \
            *opcode_p = OPCODE_PANIC;                       \
        }                                                   \
    }

#define OPCODE_LOAD(type_id, opcode_p)                      \
    switch (type_id){                                       \
        case TYPE_INT: { *opcode_p = OPCODE_ILOAD; break;}  \
        case TYPE_FLOAT: { *opcode_p = OPCODE_FLOAD; break;}\
        default: {                                          \
            printf("unable to load type %d",type_id);       \
            *opcode_p = OPCODE_PANIC;                       \
        }                                                   \
    }

#define OPCODE_CONST(type_id, opcode_p)                      \
    switch (type_id){                                        \
        case TYPE_INT: { *opcode_p = OPCODE_ICONST; break;}  \
        case TYPE_FLOAT: { *opcode_p = OPCODE_FCONST; break;}\
        default: {                                          \
            printf("unable to const type %d",type_id);      \
            *opcode_p = OPCODE_PANIC;                       \
        }                                                   \
    }

#endif //SHELL_OPCODES_H
