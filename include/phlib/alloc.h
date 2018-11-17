/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/7/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_ALLOC_H
#define PHANTOMSHELL_ALLOC_H

#include "log.h"
#include "types.h"

#ifdef __simbuild__

#include <malloc.h>

#endif //__simbuild__

namespace phlib {

/**
 * @brief OS independent wrap for malloc function
 * @param size in bytes requested to allocate
 * @return pointer to allocated memory if succeed, 0 otherwise
 */
void *malloc(size_t size);

/**
 * @brief OS independent wrap for free function
 * @param p pointer to allocated by phlib::malloc memory
 */
void free(void *p);

} //namespace phlib

/*
 * INLINE DEFINITIONS ARE BELOW
 */

inline void *phlib::malloc(size_t size) {
#ifdef __simbuild__
    void *pointer = ::malloc(size);
#ifdef __debug__
    DEBUG_LOG("alloc.h: allocated %lu bytes at address %p\n", size, pointer);
#endif //__debug__
    return pointer;
#else //__simbuild__
    //TODO: create custom malloc wrap, using phantom api
#endif //__simbuild__
}

inline void phlib::free(void *p) {
#ifdef __simbuild__
    ::free(p);
#ifdef __debug__
    DEBUG_LOG("alloc.h: released address %p\n", p);
#endif //__debug__
#else //__simbuild__
    //TODO: create free wrap for phantom api
#endif //__simbuild__
}

#endif //PHANTOMSHELL_ALLOC_H
