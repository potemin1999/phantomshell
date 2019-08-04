/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/4/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <stdio.h>
#include "opcodes.h"


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
        default: {
            printf("invalid opcode %hhu\n", opcode);
            return "!invalid opcode!";
        }
    }
}
