/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/28/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Alloc.h"

PHLIB_NAMESPACE_BEGIN

class SystemAllocator : public Allocator {
public:

    Ptr allocate(Size size) override;

    void deallocate(Ptr ptr) override;

};

Ptr SystemAllocator::allocate(Size size) {
    Ptr ptr = Malloc(size);
#   ifdef __debug__
    DEBUG_LOG_MEM("allocated %lu bytes at address %p\n", size, ptr);
#   endif //__debug__
    return ptr;
}

void SystemAllocator::deallocate(Ptr ptr) {
    Free(ptr);
#   ifdef __debug__
    DEBUG_LOG_MEM("released address %p\n", ptr);
#   endif //__debug__
}

SystemAllocator defaultAllocator;

Allocator *Allocator::getDefaultAllocator() noexcept {
    return &defaultAllocator;
}

PHLIB_NAMESPACE_END