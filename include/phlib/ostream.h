/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/15/18
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

/** @brief Default namespace for Phantom Shell support library */
namespace phlib {

/**
 * @brief OStream represents Output Stream to some location
 */
class OStream {

    /**
     * @brief Function pointer to exact write function implementation
     */
    typedef SSize (OStream::*WriteFunc)(ConstPtr, Size);

    /**
     * @brief Function pointer to exact close function implementation
     */
    typedef int (OStream::*CloseFunc)();

    /**
    * @brief This union stores data for each type of istream
    */
    typedef union OStreamData {
#ifdef __simbuild__
        File *file; /**< Stores output file descriptor, when type==FILE_STREAM*/
#endif //__simbuild__
        struct {
            Ptr object; /**< Stores pointer to destination object, when type==OBJECT_STREAM*/
            bool object_copy;
            SSize current_pointer;
            SSize object_size; /**< Stores destination object size, when type==OBJECT_STREAM*/
        };
        int stdout_fd; /**< Stores stdout descriptor, when type==STDOUT_STREAM*/
    } OStreamData;

    /**
    * @brief This enum describes istream types
    */
    typedef enum OStreamType {
        FILE_STREAM = 0,
        STDOUT_STREAM = 1,
        OBJECT_STREAM = 2
    } OStreamType;

protected:

    WriteFunc write_func;
    CloseFunc close_func;
    OStreamType type;
    OStreamData data;
    bool isClosed = false;

public:

#ifdef __simbuild__

    /**
     * @brief Creates stream, writing to the file
     * @param file_path
     */
    OStream(String &file_path) {
        type = OStreamType::FILE_STREAM;
        write_func = &OStream::write_to_file;
        close_func = &OStream::close_file;
        data.file = fopen(file_path, "w+");
    }

#endif //__simbuild__

    /**
     * @brief Creates stream, writing to pointed buffer
     * @param buffer
     */
    OStream(Ptr buffer) {
        //TODO: implement later
    }

    /**
     * @brief Creates stream, writing to stdout
     */
    OStream() {
        type = OStreamType::STDOUT_STREAM;
        write_func = &OStream::write_to_stdout;
        close_func = &OStream::close_stdout;
        data.stdout_fd = open("/dev/stdout", OFlags::WRONLY);
        DEBUG_LOG("created new stdout output stream with stdout_fd = %d\n", data.stdout_fd);
    }

    /**
     * @brief Only destructor of OStream
     *
     * Also closes resources if they we not released earlier
     */
    ~OStream() {
        if (!isClosed) {
            close();
        }
    }

    /**
     * @brief Allocates memory for new OStream
     * @param size of OStream object
     * @return memory pointer
     */
    Ptr operator new(Size size) {
        return phlib::malloc(size);
    }

    /**
     * @brief Frees memory from OStream
     * @param pointer to OStream object
     */
    void operator delete(Ptr pointer) {
        phlib::free(pointer);
    }

    /**
     * @brief Writes data to location, described at creation
     * @param buffer to get data from
     * @param buffer_size is size of @p buffer in bytes
     * @return actually written bytes count
     */
    SSize write(ConstPtr buffer, Size buffer_size) {
        return (this->*write_func)(buffer, buffer_size);
    }

    /**
     * @brief Closes resources in use
     * @return -1 if it was already closed
     * @return value of close function implementation
     */
    int close() {
        if (isClosed) return -1;
        return (this->*close_func)();
    }

private:

    SSize write_to_stdout(ConstPtr buffer, Size buffer_size) {
        return phlib::write(data.stdout_fd, buffer, buffer_size);
    }

#ifdef __simbuild__

    SSize write_to_file(ConstPtr buffer, Size buffer_size) {
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
