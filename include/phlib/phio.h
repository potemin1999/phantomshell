/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/16/18
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PHLIB_H
#define PHANTOMSHELL_PHLIB_H

#include "types.h"

#ifdef __simbuild__

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

typedef FILE file_t;

#endif

namespace phlib {

#ifdef __simbuild__
typedef enum o_flags {
    RDONLY = O_RDONLY,
    WRONLY = O_WRONLY,
    RDWR = O_RDWR
} o_flags;
#else //__simbuild__
typedef enum o_flags {
} o_flags;
#endif //__simbuild__

int open(const char *file, o_flags flag);

ssize_t read(int fd, void *buffer, size_t buffer_size);

ssize_t write(int fd, const void *buffer, size_t nbytes);

int close(int fd);

#ifdef __simbuild__

file_t *fopen(const char *file, const char *modes);

ssize_t fread(file_t *file, void *buffer, size_t buffer_size);

ssize_t fwrite(file_t *file, const void *buffer, size_t nbytes);

int fclose(file_t *file);

#endif //__simbuild__

} //namespace phlib

/*
 * INLINE DEFINITIONS ARE BELOW
 */
#ifdef __simbuild__

inline int phlib::open(const char *file, phlib::o_flags flag) {
    return ::open(file, flag);
}

inline ssize_t phlib::read(int fd, void *buffer, size_t buffer_size) {
    return ::read(fd, buffer, buffer_size);
}

inline ssize_t phlib::write(int fd, const void *buffer, size_t nbytes) {
    return ::write(fd, buffer, nbytes);
}

inline int phlib::close(int fd) {
    return ::close(fd);
}

inline file_t *phlib::fopen(const char *file, const char *modes) {
    return ::fopen(file, modes);
}

inline ssize_t phlib::fread(file_t *file, void *buffer, size_t buffer_size) {
    return ::fread(buffer, 1, buffer_size, file);
}

inline ssize_t phlib::fwrite(file_t *file, const void *buffer, size_t nbytes) {
    return ::fwrite(buffer, 1, nbytes, file);
}

inline int phlib::fclose(file_t *file) {
    return ::fclose(file);
}

#else //__simbuild__

//TODO: implement next function according to phantom api
inline int phlib::open(const char *file, o_flags flags) {
}

inline ssize_t phlib::read(int fd,void* buffer,size_t buffer_size){
}

inline ssize_t phlib::write(int fd,const void* buffer,size_t nbytes){
}

inline int phlib::close(int fd){
}

#endif //__simbuild__

#endif //PHANTOMSHELL_PHLIB_H
