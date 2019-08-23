/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/5/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef SHELL_RUNTIME_H
#define SHELL_RUNTIME_H

// type - 64 bits
#define RUNTIME_TYPE_BOOL   0x0000000000000001
#define RUNTIME_TYPE_INT    0x0000000000000002
#define RUNTIME_TYPE_FLOAT  0x0000000000000003
#define RUNTIME_TYPE_CHAR   0x0000000000000004
#define RUNTIME_TYPE_STRING 0x0000000000000005
#define RUNTIME_TYPE_VOID   0x0000000000000006

typedef struct object_ref_t {
    runtime_type_t type;
} object_ref_t;

typedef struct {
} psh_class_desc_t;

int runtime_define_class(psh_class_desc_t *type_desc);

#endif //SHELL_RUNTIME_H
