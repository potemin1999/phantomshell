/**
 * Created by Ilya Potemin on 11/14/18.
 * Part of Phantom Shell project.
 */

#ifndef PHANTOMSHELL_TYPES_H
#define PHANTOMSHELL_TYPES_H

#ifdef __simbuild__
#ifdef __unix__

#include <sys/types.h>

#endif //__unix__
#else
#include <phantom_types.h>
#endif //__simbuild__

#endif //PHANTOMSHELL_TYPES_H
