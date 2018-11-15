/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/14/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_LOG_H
#define PHANTOMSHELL_LOG_H

#ifdef __debug__
#ifdef __simbuild__

#include <cstdio>
#include <cstdarg>

#endif //__simbuild__
#endif


namespace phlib {

#ifdef __debug__

/**
 * @brief Prints formatted string, if compiled with debug mode enabled
 * @param format string for arguments
 * @param ... format args
 */
void debug_log_func(const char *file, unsigned line, const char *format, ...) {
#ifdef __simbuild__
    std::printf("%s:%u\n", file, line);
    std::va_list list;
    va_start(list, format);
    std::vprintf(format, list);
    va_end(list);

#endif //__simbuild__
}

#endif //__debug__

#ifdef __debug__
#ifdef __simbuild__
#define DEBUG_LOG(...); debug_log_func(__FILE__,__LINE__,__VA_ARGS__)
#endif //__simbuild__
#else  //__debug__
#define DEBUG_LOG(...);
#endif //__debug__

}
#endif //PHANTOMSHELL_LOG_H
