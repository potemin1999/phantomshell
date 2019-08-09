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

#include "../operator.h"

typedef unsigned char opcode_t;

/** Does nothing */
#define OPCODE_NOP      0x00
#define OPCODE_EXSTK    0x10 // 16 // expands stack capacity

/** LOAD or SAVE
 * puts/loads something from/to local variable storage
 * [1]  0 - root scope
 *      1 - method scope
 * [7] index
 */
#define OPCODE_ILOAD    0x32 // 50 // ( -> value ) : loads integer
#define OPCODE_ISAVE    0x33 // 51 // ( value -> ) : saves integer
#define OPCODE_FLOAD    0x34 // 52 // ( value -> ) : loads float
#define OPCODE_FSAVE    0x35 // 53 // ( value -> ) : saves float
/**
* CONST puts value on the stack
* B = [1] byte
* I = [4] bytes
* F = [4] bytes
*/
#define OPCODE_ICONST   0x36 // 54 // ( -> value ) : puts integer on the stack
#define OPCODE_FCONST   0x37 // 55 // ( -> value ) : puts float on the stack

#define OPCODE_IADD     0x42 // 66 // ( value_1,value_2 -> value_3) : value_1 + value_2 = value_3, integers
#define OPCODE_ISUB     0x43 // 67 // ( value_1,value_2 -> value_3) : value_1 - value_2 = value_3, integers
#define OPCODE_IMUL     0x44 // 68 // ( value_1,value_2 -> value_3) : value_1 * value_2 = value_3, integers
#define OPCODE_IDIV     0x45 // 69 // ( value_1,value_2 -> value_3) : value_1 / value_2 = value_3, integers
#define OPCODE_INEG     0x46 // 70 // ( value -> result ) : negates integer on the stack
#define OPCODE_I2F      0x47 // 71 // ( value -> result ) : transforms integer to the float
#define OPCODE_IEQ      0x4a // 74 // ( v_1, v_2 -> r_3) : if value_1==value_2, puts 1 on stack, else 0
#define OPCODE_INEQ     0x4b // 75 // ( v_1, v_2 -> r_3) : if value_1!=value_2, puts 1 on stack, else 0

#define OPCODE_FADD     0x50 // 80 // ( value_1,value_2 -> value_3) : value_1 + value_2 = value_3, floats
#define OPCODE_FSUB     0x51 // 81 // ( value_1,value_2 -> value_3) : value_1 - value_2 = value_3, floats
#define OPCODE_FMUL     0x52 // 82 // ( value_1,value_2 -> value_3) : value_1 * value_2 = value_3, floats
#define OPCODE_FDIV     0x53 // 83 // ( value_1,value_2 -> value_3) : value_1 / value_2 = value_3, floats
#define OPCODE_FNEG     0x54 // 84 // (value -> result ) : negates float on the stack

// (v_1 -> ) : int operand on the stack, moves pc by offset written in two next bytes
#define OPCODE_JMP      0x60 // 96 // moves pc always
#define OPCODE_LJMP     0x61 // 97 // moves pc, offset written if 4 bytes
#define OPCODE_JEZ      0x62 // 98 // moves pc if operand==0
#define OPCODE_JNEZ     0x63 // 99 // moves pc if operand!=0

#define OPCODE_PANIC    0xff // stops execution

const char *get_opcode_name(opcode_t opcode);

opcode_t int_operator_to_opcode(int oper);

opcode_t float_operator_to_opcode(int oper);

#define OPCODE_SAVE(type_id, opcode_p)                      \
    switch (type_id){                                  \
        case TYPE_BOOL:\
        case TYPE_INT: { *(opcode_p) = OPCODE_ISAVE; break;}  \
        case TYPE_FLOAT: { *(opcode_p) = OPCODE_FSAVE; break;}\
        default: {                                          \
            printf("%s %d : unable to save type %lu",__FILE__,__LINE__,type_id);       \
            *(opcode_p) = 0;                                  \
        }                                                   \
    }

#define OPCODE_LOAD(type_id, opcode_p)                      \
    switch (type_id){                                       \
        case TYPE_BOOL:\
        case TYPE_INT: { *(opcode_p) = OPCODE_ILOAD; break;}  \
        case TYPE_FLOAT: { *(opcode_p) = OPCODE_FLOAD; break;}\
        default: {                                          \
            printf("unable to load type %lu",type_id);      \
            *(opcode_p) = 0;                                  \
        }                                                   \
    }

#define OPCODE_CONST(type_id, opcode_p)                      \
    switch (type_id){                                        \
        case TYPE_BOOL:\
        case TYPE_INT: { *(opcode_p) = OPCODE_ICONST; break;}  \
        case TYPE_FLOAT: { *(opcode_p) = OPCODE_FCONST; break;}\
        default: {                                          \
            printf("unable to const type %lu",type_id);     \
            *(opcode_p) = 0;                                  \
        }                                                   \
    }

#endif //SHELL_OPCODES_H
