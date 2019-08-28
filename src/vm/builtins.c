#include "lib.h"
#include "vm.h"

/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/28/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

const char *psh_lang_panic_signature = "psh_panic(I)";
const uint8_t psh_lang_panic_body[] = {
        0, 0x1, /*1 local var*/
        0, 0x0, /* 0 bytes stack size*/
        0xff, /* call panic opcode */
};

#define LOAD_BUILTIN_FUNC(signature, body) {            \
    vm_pool_const_t sigindex = 0;                   \
    vm_func_handle_t *handle = 0;                   \
    vm_register_constant(strlen(signature)+1,       \
            (signature),&sigindex);                 \
    vm_register_function(sigindex,                  \
            sizeof(body),(void*) (body),&handle);   \
    }

int vm_load_builtin_functions() {
    LOAD_BUILTIN_FUNC(psh_lang_panic_signature, psh_lang_panic_body)
    return 0;
}
