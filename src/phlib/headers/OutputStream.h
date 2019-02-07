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

#include "Log.h"
#include "String.h"
#include "Types.h"
#include "PhantomIO.h"

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
    union OStreamData;

    /**
    * @brief This enum describes istream types
    */
    enum OStreamType {
        FILE_STREAM   = 0,
        STDOUT_STREAM = 1,
        OBJECT_STREAM = 2
    };

protected:

    WriteFunc   writeFunc;
    CloseFunc   closeFunc;
    OStreamType type;
    OStreamData *data;
    bool        isClosed = false;

public:

#ifdef __simbuild__

    /**
     * @brief Creates stream, writing to the file
     * @param filePath
     */
    OStream(String &filePath);

#endif //__simbuild__

    /**
     * @brief Creates stream, writing to pointed buffer
     * @param buffer
     */
    OStream(Ptr buffer);

    /**
     * @brief Creates stream, writing to stdout
     */
    OStream();

    /**
     * @brief Only destructor of OStream
     *
     * Also closes resources if they we not released earlier
     */
    ~OStream();

    /**
     * @brief Allocates memory for new OStream
     * @param size of OStream object
     * @return memory pointer
     */
    Ptr operator new(Size size);

    /**
     * @brief Frees memory from OStream
     * @param pointer to OStream object
     */
    void operator delete(Ptr pointer);

    /**
     * @brief Writes data to location, described at creation
     * @param buffer to get data from
     * @param bufferSize is size of @p buffer in bytes
     * @return actually written bytes count
     */
    SSize write(ConstPtr buffer, Size bufferSize);

    /**
     * @brief Closes resources in use
     * @return -1 if it was already closed
     * @return value of close function implementation
     */
    int close();

private:

    SSize writeToStdout(ConstPtr buffer, Size bufferSize);

#ifdef __simbuild__

    SSize writeToFile(ConstPtr buffer, Size buffer_size);

#endif //__simbuild__

    int closeStdout();

#ifdef __simbuild__

    int closeFile();

#endif //__simbuild__

};

} //namespace phlib
#endif //PHANTOMSHELL_OSTREAM_H
