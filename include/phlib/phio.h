/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/16/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_PHIO_H
#define PHANTOMSHELL_PHIO_H

#include "types.h"
#include "string.h"

#ifdef __simbuild__

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

typedef FILE File;

#endif

/** @brief Default namespace for Phantom Shell support library */
namespace phlib {

#ifdef __simbuild__
typedef enum OFlags {
    RDONLY = O_RDONLY,
    WRONLY = O_WRONLY,
    RDWR = O_RDWR
} OFlags;
#else //__simbuild__
typedef enum OFlags {
} OFlags;
#endif //__simbuild__

int print(String str);

int open(String file, OFlags flag);

SSize read(int fd, Ptr buffer, Size buffer_size);

SSize write(int fd, ConstPtr buffer, Size nbytes);

int close(int fd);

#ifdef __simbuild__

File *fopen(String file, const char *modes);

SSize fread(File *file, Ptr buffer, Size buffer_size);

SSize fwrite(File *file, ConstPtr buffer, Size nbytes);

int fclose(File *file);

#endif //__simbuild__

} //namespace phlib

/*
 * INLINE DEFINITIONS ARE BELOW
 */
#ifdef __simbuild__

inline int phlib::print(String str){
    return ::printf("%s",str.char_value());
}

inline int phlib::open(String file, phlib::OFlags flag) {
    return ::open(file.char_value(), flag);
}

inline SSize phlib::read(int fd, Ptr buffer, Size buffer_size) {
    return ::read(fd, buffer, buffer_size);
}

inline SSize phlib::write(int fd, ConstPtr buffer, Size nbytes) {
    return ::write(fd, buffer, nbytes);
}

inline int phlib::close(int fd) {
    return ::close(fd);
}

inline File *phlib::fopen(String file, const char *modes) {
    return ::fopen(file.char_value(), modes);
}

inline SSize phlib::fread(File *file, Ptr buffer, Size buffer_size) {
    return ::fread(buffer, 1, buffer_size, file);
}

inline SSize phlib::fwrite(File *file, ConstPtr buffer, Size nbytes) {
    return ::fwrite(buffer, 1, nbytes, file);
}

inline int phlib::fclose(File *file) {
    return ::fclose(file);
}

#else //__simbuild__

//TODO: implement next function according to phantom api

inline int phlib::print(String str){
}

inline int phlib::open(const char *file, OFlags flags) {
}

inline SSize phlib::read(int fd,Ptr buffer,Size buffer_size){
}

inline SSize phlib::write(int fd,ConstPtr buffer,Size nbytes){
}

inline int phlib::close(int fd){
}

#endif //__simbuild__

#endif //PHANTOMSHELL_PHIO_H
