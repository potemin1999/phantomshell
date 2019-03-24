/**
 * @headerfile
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/15/18.
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_ISTREAM_H
#define PHANTOMSHELL_ISTREAM_H

#include "Log.h"
#include "String.h"
#include "Types.h"
#include "Io.h"

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
        STDIN_STREAM  = 1,
        OBJECT_STREAM = 2
    };

protected:

    ReadFunc    readFunc;
    CloseFunc   closeFunc;
    IStreamData *data;
    IStreamType type;
    bool        isClosed = false;

public:

#ifdef __simbuild__

    /**
     * @brief Creates input stream from file
     * @param filePath
     */
    IStream(String &filePath);

#endif //__simbuild__

    /**
     * @brief Creates input stream from object
     * @param byteBuffer to use as source
     * @param size size of buffer
     * @param doCopy determines whether it should be copied
     */
    IStream(ConstPtr byteBuffer, Size size, bool doCopy);

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
    //Ptr operator new(Size size);

    /**
     * @brief Frees memory from IStream
     * @param pointer to IStream object
     */
    //void operator delete(Ptr pointer);

    /**
     * @brief reads bytes from corresponding source
     * @param buffer place for write to
     * @param bufferSize is @p buffer size in bytes
     * @return -1 if @p bufferSize is negative
     * @return -2 if @p buffer is null and @p bufferSize if positive
     * @return  value of read implementation otherwise
     */
    SSize read(Ptr buffer, Size bufferSize);

    /**
     * @brief releases all resources, associated with this stream
     *         also called by destructor, if needed
     */
    int close();

private:

#ifdef __simbuild__

    SSize readFromFile(Ptr buffer, Size bufferSize);

#endif //__simbuild__

    SSize readFromStdin(Ptr buffer, Size bufferSize);

    SSize readFromObject(Ptr buffer, Size bufferSize);

    int closeObject();

    int closeStdin();

#ifdef __simbuild__

    int closeFile();

#endif //__simbuild__

};

} //namespace phlib
#endif //PHANTOMSHELL_ISTREAM_H
