/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/14/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_LOG_H
#define PHANTOMSHELL_LOG_H

#ifdef __debug__
#ifdef __simbuild__

#include <stdio.h>
#include <stdarg.h>

#endif //__simbuild__
#endif //__debug__

/** @brief Default namespace for Phantom Shell support library */
namespace phlib {

#ifdef __debug__

/**
 * @brief Prints formatted string, if compiled with debug mode enabled
 *
 * keyword inline let us implement this function below
 * @param format string for arguments
 * @param ... format args
 */
inline void debugLogFunc(const char *file, unsigned line, const char *format, ...);

#endif //__debug__

} //namespace phlib

#ifdef __debug__
#ifdef __simbuild__
#define DEBUG_LOG(...) phlib::debugLogFunc(__FILE__,__LINE__,__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif //__simbuild__
#else  //__debug__
#define DEBUG_LOG(...)
#endif //__debug__

/*
 *  INLINE DEFINITIONS ARE BELOW
 */

#ifdef __debug__

void phlib::debugLogFunc(const char *file, unsigned line, const char *format, ...) {
#ifdef __simbuild__
    printf("%s:%u\n", file, line);
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
#endif //__simbuild__
}

#endif //__debug__

#endif //PHANTOMSHELL_LOG_H
