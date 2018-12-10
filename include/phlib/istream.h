/**
 * @headerfile
 * @author Ilya Potemin
 * @date 11/15/18.
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

/** @brief Default namespace for Phantom Shell support library */
namespace phlib {

/**
 * @brief IStream is Input Stream and it takes input from requested location
 */
class IStream {


    /**
     * @brief Function pointer to exact read implementation
     */
    typedef SSize (IStream::*ReadFunc)(Ptr, Size);

    /**
     * @brief Function pointer to exact close function implementation
     */
    typedef int (IStream::*CloseFunc)();

    /**
     * @brief This union stores data for each type of IStream
     */
    union IStreamData;

    /**
     * @brief This enum describes IStream types
     */
    enum IStreamType {
#ifdef __simbuild__
        FILE_STREAM = 0,
#endif //__simbuild__
        STDIN_STREAM = 1,
        OBJECT_STREAM = 2
    };

protected:

    ReadFunc read_func;
    CloseFunc close_func;
    IStreamData* data;
    IStreamType type;
    bool isClosed = false;

public:

#ifdef __simbuild__

    /**
     * @brief Creates input stream from file
     * @param file_path
     */
    IStream(String &file_path);

#endif //__simbuild__

    /**
     * @brief Creates input stream from object
     * @param byte_buffer to use as source
     * @param size size of buffer
     * @param do_copy determines whether it should be copied
     */
    IStream(ConstPtr byte_buffer, Size size, bool do_copy);

    /**
     * @brief Creates input stream from stdin
     */
    IStream();

    /**
     * @brief unique destructor
     *
     * Also closes resources if they we not released earlier
     */
    ~IStream();

    /**
     * @brief Allocates memory for new IStream
     * @param size of IStream object
     * @return memory pointer to allocated memory
     */
    Ptr operator new(Size size);

    /**
     * @brief Frees memory from IStream
     * @param pointer to IStream object
     */
    void operator delete(Ptr pointer);

    /**
     * @brief reads bytes from corresponding source
     * @param buffer place for write to
     * @param buffer_size is @p buffer size in bytes
     * @return -1 if @p buffer_size is negative
     * @return -2 if @p buffer is null and @p buffer_size if positive
     * @return  value of read implementation otherwise
     */
    SSize read(Ptr buffer, Size buffer_size);

    /**
     * @brief releases all resources, associated with this stream
     *         also called by destructor, if needed
     */
    int close();

private:

#ifdef __simbuild__
    SSize read_from_file(Ptr buffer, Size buffer_size);
#endif //__simbuild__

    SSize read_from_stdin(Ptr buffer, Size buffer_size);

    SSize read_from_object(Ptr buffer, Size buffer_size) ;

    int close_object();

    int close_stdin() ;

#ifdef __simbuild__
    int close_file();
#endif //__simbuild__

};

} //namespace phlib
#endif //PHANTOMSHELL_ISTREAM_H
