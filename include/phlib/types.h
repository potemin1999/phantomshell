/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/14/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
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
