/**
 * @headerfile
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/7/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_ALLOC_H
#define PHANTOMSHELL_ALLOC_H

#include "Log.h"
#include "Types.h"

#ifdef __simbuild__

#include <malloc.h>

#endif //__simbuild__

#ifdef __debug_memory__
#define DEBUG_LOG_MEM(...) DEBUG_LOG(__VA_ARGS__)
#else
#define DEBUG_LOG_MEM(...)
#endif

PHLIB_NAMESPACE_BEGIN

/**
 * @brief OS independent wrap for malloc function
 * @param size in bytes requested to allocate
 * @return pointer to allocated memory if succeed, 0 otherwise
 */
Ptr malloc(Size size);

/**
 * @brief OS independent wrap for free function
 * @param p pointer to allocated by phlib::malloc memory
 */
void free(Ptr p);


/**
 * @brief parent class for all allocators
 */
class Allocator {
public:

    Allocator() = default;

    ~Allocator() = default;

    /**
     * @brief allocates memory of required size
     *
     * Only implementation can declare how to allocate memory
     * @param size in bytes to allocate
     * @return nullptr if allocation was failed
     */
    virtual Ptr allocate(Size size) = 0;

    /**
     * @brief deallocate memory
     *
     * Like the phlib::Allocate::allocate, this one does not declare
     * how to deallocate and release memory to the system
     * @param ptr is and pointer to previously allocated memory
     */
    virtual void deallocate(Ptr ptr) = 0;

    virtual void release() {}

    static Allocator *getDefaultAllocator() noexcept;

};


PHLIB_NAMESPACE_END

#endif //PHANTOMSHELL_ALLOC_H
