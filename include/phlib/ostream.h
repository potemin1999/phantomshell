/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/15/18
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_OSTREAM_H
#define PHANTOMSHELL_OSTREAM_H

#include "log.h"
#include "string.h"
#include "types.h"
#include "phio.h"

namespace phlib {

/**
 * @class ostream represents Output Stream to some location
 */
class ostream {

    typedef ssize_t (ostream::*write_func_t)(const void *, size_t);

    typedef int (ostream::*close_func_t)();

    /**
    * @brief This union stores data for each type of istream
    */
    typedef union ostream_data_t {
#ifdef __simbuild__
        file_t *file; /**< Stores output file descriptor, when type==FILE_STREAM*/
#endif //__simbuild__
        struct {
            void *object; /**< Stores pointer to destination object, when type==OBJECT_STREAM*/
            bool object_copy;
            ssize_t current_pointer;
            ssize_t object_size; /**< Stores destination object size, when type==OBJECT_STREAM*/
        };
        int stdout_fd; /**< Stores stdout descriptor, when type==STDOUT_STREAM*/
    } ostream_data_t;

    /**
    * @brief This enum describes istream types
    */
    typedef enum ostream_type_t {
        FILE_STREAM = 0,
        STDOUT_STREAM = 1,
        OBJECT_STREAM = 2
    } ostream_type_t;

protected:

    write_func_t write_func;
    close_func_t close_func;
    ostream_type_t type;
    ostream_data_t data;
    bool isClosed = false;

public:

#ifdef __simbuild__

    /**
     * @brief Creates stream, writing to the file
     * @param file_path
     */
    ostream(string &file_path) {
        type = ostream_type_t::FILE_STREAM;
        write_func = &ostream::write_to_file;
        close_func = &ostream::close_file;
        data.file = fopen(file_path.value(), "w+");
    }

#endif //__simbuild__

    /**
     * @brief Creates stream, writing to pointed buffer
     * @param buffer
     */
    ostream(void *buffer) {
        //TODO: implement later
    }

    /**
     * @brief Creates stream, writing to stdout
     */
    ostream() {
        type = ostream_type_t::STDOUT_STREAM;
        write_func = &ostream::write_to_stdout;
        close_func = &ostream::close_stdout;
        data.stdout_fd = open("/dev/stdout", o_flags::WRONLY);
        DEBUG_LOG("created new stdout output stream with stdout_fd = %d\n", data.stdout_fd);
    }

    ~ostream() {
        if (!isClosed) {
            close();
        }
    }

    void *operator new(size_t size) {
        return phlib::malloc(size);
    }

    void operator delete(void *pointer) {
        phlib::free(pointer);
    }

    ssize_t write(const void *buffer, size_t buffer_size) {
        return (this->*write_func)(buffer, buffer_size);
    }

    int close() {
        if (isClosed) return -1;
        return (this->*close_func)();
    }

private:

    ssize_t write_to_stdout(const void *buffer, size_t buffer_size) {
        return phlib::write(data.stdout_fd, buffer, buffer_size);
    }

#ifdef __simbuild__

    ssize_t write_to_file(const void *buffer, size_t buffer_size) {
        return phlib::fwrite(data.file, buffer, buffer_size);
    }

#endif //__simbuild__

    int close_stdout() {
        phlib::close(data.stdout_fd);
    }

#ifdef __simbuild__

    int close_file() {
        phlib::fclose(data.file);
    }

#endif //__simbuild__

};

} //namespace phlib
#endif //PHANTOMSHELL_OSTREAM_H
