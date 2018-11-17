/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/15/18.
 * @section LICENSE
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_ISTREAM_H
#define PHANTOMSHELL_ISTREAM_H

#include "log.h"
#include "string.h"
#include "types.h"
#include "phio.h"

namespace phlib {

/**
 * @class istream is Input Stream and it takes input from requested location
 */
class istream {

    /**
     * @brief Function pointer to exact read implementation
     */
    typedef ssize_t (istream::*read_func_t)(void *, size_t);

    typedef int (istream::*close_func_t)();

    /**
     * @brief This union stores data for each type of istream
     */
    typedef union istream_data_t {
#ifdef __simbuild__
        file_t *file; /**< Stores input file descriptor, when type==FILE_STREAM*/
#endif //__simbuild__
        struct {
            void *object; /**< Stores pointer to source object, when type==OBJECT_STREAM*/
            bool object_copy;
            ssize_t current_pointer;
            ssize_t object_size; /**< Stores source object size, when type==OBJECT_STREAM*/
        };
        int stdin_fd; /**< Stores stdin descriptor, when type==STDIN_STREAM*/
    } istream_data_t;

    /**
     * @brief This enum describes istream types
     */
    typedef enum istream_type_t {
#ifdef __simbuild__
        FILE_STREAM = 0,
#endif //__simbuild__
        STDIN_STREAM = 1,
        OBJECT_STREAM = 2
    } istream_type_t;

protected:

    read_func_t read_func;
    close_func_t close_func;
    istream_data_t data;
    istream_type_t type;
    bool isClosed = false;

public:

#ifdef __simbuild__

    /**
     * @brief Creates input stream from file
     * @param file_path
     */
    istream(string &file_path) {
        type = istream_type_t::FILE_STREAM;
        read_func = &istream::read_from_file;
        close_func = &istream::close_file;
        data.file = phlib::fopen(file_path.value(), "r");
    }

#endif //__simbuild__

    /**
     * @brief Creates input stream from object
     * @param byte_buffer to use as source
     * @param do_copy determines whether it should be copied
     */
    istream(void *byte_buffer, size_t size, bool do_copy) {
        type = istream_type_t::OBJECT_STREAM;
        read_func = &istream::read_from_object;
        close_func = &istream::close_object;
        if (do_copy) {
            data.object = phlib::malloc(size);
            for (int i = 0; i < size; ((char *) data.object)[i] = ((char *) byte_buffer)[i], i++);
        } else {
            data.object = byte_buffer;
        }
        data.object_copy = do_copy;
        data.object_size = size;
        data.current_pointer = 0;
    }

    /**
     * @brief Creates input stream from stdin
     */
    istream() {
        type = istream_type_t::STDIN_STREAM;
        read_func = &istream::read_from_stdin;
        close_func = &istream::close_stdin;
        data.stdin_fd = phlib::open("/dev/stdin", o_flags::RDONLY);
        DEBUG_LOG("created input stream from stdin with stdin_fd = %d\n", data.stdin_fd);
    }

    /**
     * @brief unique destructor
     */
    ~istream() {
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

    /**
     * @brief reads bytes from corresponding source
     * @param buffer place for write to
     * @param buffer_size is @p buffer size in bytes
     * @return -1 if @p buffer_size is negative
     * @return -2 if @p buffer is <tt>null<tt/> and @p buffer_size if positive
     * @return  value of read implementation otherwise
     */
    ssize_t read(void *buffer, size_t buffer_size) {
        if (buffer_size < 0) return -1;
        if (buffer == nullptr && buffer_size > 0) return -2;
        return (this->*read_func)(buffer, buffer_size);
    }

    /**
     * @brief releases all resources, associated with this stream
     *         also called by destructor, if needed
     */
    int close() {
        if (isClosed) return -1;
        return (this->*close_func)();
    }

private:

#ifdef __simbuild__

    ssize_t read_from_file(void *buffer, size_t buffer_size) {
        return phlib::fread(data.file, buffer, buffer_size);
    }

#endif //__simbuild__

    ssize_t read_from_stdin(void *buffer, size_t buffer_size) {
        return phlib::read(data.stdin_fd, buffer, buffer_size);
    }

    ssize_t read_from_object(void *buffer, size_t buffer_size) {
        ssize_t read_size = buffer_size < (data.object_size - data.current_pointer - 1) ?
                            buffer_size : data.object_size - data.current_pointer - 1;
        for (int i = 0; i < read_size; ((char *) buffer)[i++] = ((char *) data.object)[data.current_pointer++]);
        return read_size;
    }

    int close_object() {
        if (data.object_copy) {
            phlib::free(data.object);
        }
    }

    int close_stdin() {
        phlib::close(data.stdin_fd);
    }

#ifdef __simbuild__

    int close_file() {
        phlib::fclose(data.file);
    }

#endif //__simbuild__

};

} //namespace phlib
#endif //PHANTOMSHELL_ISTREAM_H
