/**
 * Created by Ilya Potemin on 11/7/18.
 * Part of Phantom Shell project.
 */

#ifndef PHANTOMSHELL_ALLOC_H
#define PHANTOMSHELL_ALLOC_H

#include "types.h"

namespace std {
#ifdef __simbuild__

#include <malloc.h>

#endif //__simbuild__
}

namespace phlib {

inline void *malloc(size_t size) {
#ifdef __simbuild__
    return std::malloc(size);
#endif //__simbuild__
}

inline void free(void *p) {
#ifdef __simbuild__
    std::free(p);
#endif
}

}
#endif //PHANTOMSHELL_ALLOC_H
