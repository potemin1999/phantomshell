/**
 * @headerfile
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 12/15/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PHLIB_H
#define PHANTOMSHELL_PHLIB_H

#include "Alloc.h"
#include "Types.h"

/** @brief Default phlib namespace */
namespace phlib {

/**
 * @brief Copies bytes from one location to another
 * 
 * Implemented using word-by-word copying where it is available
 * @note  Does not handle overlapping memory areas
 * @param dst Pointer to destination memory area
 * @param src Pointer to source memory
 * @param num Number of bytes to copy
 * @return @p destination
 */
Ptr memcpy(Ptr dst, ConstPtr src, Size num);

/**
 * @brief Moves bytes from one location to another
 * @note  Handles overlapping memory areas
 * @param dst Pointer to destination memory
 * @param src Pointer to source memory
 * @param num Number of bytes to move
 * @return
 */
Ptr memmove(Ptr dst, ConstPtr src, Size num);

}

inline Ptr phlib::memcpy(Ptr dst, ConstPtr src, Size num) {
    Size i = 0;
    auto wordDst = (WordPtr) dst;
    auto wordSrc = (ConstWordPtr) src;
    for (i = 0; i < num / WORDSIZE; i++) {
        wordDst[i] = wordSrc[i];
    }
    i *= 4;
    if (i - 1 == num) return dst;
    auto byteDst = (BytePtr) dst;
    auto byteSrc = (ConstBytePtr) src;
    for (i -= WORDSIZE; i < num; i++) {
        byteDst[i] = byteSrc[i];
    }
    return dst;
}

inline Ptr phlib::memmove(Ptr dst, ConstPtr src, Size num) {
    if (dst == src)
        return dst;
    auto byteDst = (BytePtr) dst;
    auto byteSrc = (ConstBytePtr) src;
    bool overlap = (byteSrc + num > byteDst) &
                   (byteSrc + num < byteDst + num);
    auto buffer = src;
    if (overlap) {
        Ptr buffer1 = (Ptr) phlib::malloc(num);
        memcpy(buffer1, src, num);
        buffer = buffer1;
    }
    memcpy(dst, buffer, num);
    return dst;
}

#endif //PHANTOMSHELL_PHLIB_H
