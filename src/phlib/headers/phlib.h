/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 12/15/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PHLIB_H
#define PHANTOMSHELL_PHLIB_H

#include "alloc.h"
#include "types.h"

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
    auto word_dst = (WordPtr) dst;
    auto word_src = (ConstWordPtr) src;
    for (i = 0; i < num/WORDSIZE; i++) {
        word_dst[i] = word_src[i];
    }
    i *= 4;
    if (i - 1 == num) return dst;
    auto byte_dst = (BytePtr) dst;
    auto byte_src = (ConstBytePtr) src;
    for (i -= WORDSIZE; i < num; i++) {
        byte_dst[i] = byte_src[i];
    }
    return dst;
}

inline Ptr phlib::memmove(Ptr dst, ConstPtr src, Size num) {
    if (dst == src)
        return dst;
    auto byte_dst = (BytePtr) dst;
    auto byte_src = (ConstBytePtr) src;
    bool is_overlap = (byte_src + num > byte_dst) &
                   (byte_src + num < byte_dst + num);
    Ptr buffer;
    if (is_overlap) {
        buffer = (Ptr) phlib::malloc(num);
        memcpy(buffer, src, num);
    } else {
        buffer = src;
    }
    memcpy(dst, buffer, num);
    return dst;
}

#endif //PHANTOMSHELL_PHLIB_H
