/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdio.h>
#include "vm/opcodes.h"


const char *get_opcode_name(opcode_t opcode) {
    switch (opcode) {
        case OPCODE_NOP: return "nop";
        case OPCODE_EXSTK: return "exstk";

        case OPCODE_ICONST: return "iconst";
        case OPCODE_ILOAD: return "iload";
        case OPCODE_ISAVE: return "isave";
        case OPCODE_IADD: return "isum";
        case OPCODE_ISUB: return "isub";
        case OPCODE_IMUL: return "imul";
        case OPCODE_IDIV: return "idiv";
        case OPCODE_INEG: return "ineg";
        case OPCODE_I2F: return "i2f";

        case OPCODE_IEQ: return "ieq";
        case OPCODE_INEQ: return "ineq";
        case OPCODE_INLT: return "inlt";
        case OPCODE_INGT: return "ingt";
        case OPCODE_ILT: return "ilt";
        case OPCODE_IGT: return "igt";

        case OPCODE_FCONST: return "fconst";
        case OPCODE_FLOAD: return "fload";
        case OPCODE_FSAVE: return "fsave";
        case OPCODE_FADD: return "fadd";
        case OPCODE_FSUB: return "fsub";
        case OPCODE_FMUL: return "fmul";
        case OPCODE_FDIV: return "fdiv";
        case OPCODE_FNEG: return "fneg";

        case OPCODE_JMP: return "jmp";
        case OPCODE_JEZ: return "jez";
        case OPCODE_JNEZ: return "jnez";
        default: {
            printf("invalid opcode %hhu\n", opcode);
            return "!invalid opcode!";
        }
    }
}
