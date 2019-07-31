/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Alloc.h"

#ifdef __simbuild__

#include <malloc.h>

#endif //__simbuild__

Ptr phlib::Malloc(Size size) {
#   ifdef __simbuild__
    Ptr pointer = ::malloc(size);
    return pointer;
#   else //__simbuild__
    //TODO: create custom malloc wrap, using phantom api
#   endif //__simbuild__
}


void phlib::Free(Ptr p) {
#   ifdef __simbuild__
    ::free(p);
#   else //__simbuild__
    //TODO: create free wrap for phantom api
#   endif //__simbuild__
}

void *operator new(Size size) {
    return phlib::Malloc(size);
}

void *operator new[](Size size) {
    return phlib::Malloc(size);
}

void operator delete(void *what) noexcept {
    phlib::Free(what);
}

void operator delete[](void *what) noexcept {
    phlib::Free(what);
}
