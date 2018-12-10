/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <phlib/alloc.h>


Ptr phlib::malloc(Size size) {
#ifdef __simbuild__
    Ptr pointer = ::malloc(size);
    return pointer;
#else //__simbuild__
    //TODO: create custom malloc wrap, using phantom api
#endif //__simbuild__
}


void phlib::free(Ptr p) {
#ifdef __simbuild__
    ::free(p);
#else //__simbuild__
    //TODO: create free wrap for phantom api
#endif //__simbuild__
}