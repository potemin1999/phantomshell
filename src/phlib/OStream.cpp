/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "OStream.h"

using namespace phlib;

//@formatter:off
union OStream::OStreamData {
#ifdef __simbuild__
    /** Stores output file descriptor, when type==FILE_STREAM */
    File file;
#endif //__simbuild__
    struct {
        /** Stores pointer to destination object, when type==OBJECT_STREAM*/
        Ptr   object;
        bool  objectCopy;
        SSize currentPointer;
        /** Stores destination object size, when type==OBJECT_STREAM*/
        SSize objectSize;
    };
    /** Stores stdout descriptor, when type==STDOUT_STREAM*/
    int  stdoutFd;
};
//@formatter:on


OStream::OStream(String &filePath) {
    data = new OStreamData();
    type = OStreamType::FILE_STREAM;
    writeFunc = &OStream::writeToFile;
    closeFunc = &OStream::closeFile;
    data->file = FOpen(filePath, "w+");
}


OStream::OStream(Ptr buffer) {
    UNUSED(buffer)
    //TODO: implement later
}


OStream::OStream() {
    data = new OStreamData();
    type = OStreamType::STDOUT_STREAM;
    writeFunc = &OStream::writeToStdout;
    closeFunc = &OStream::closeStdout;
    data->stdoutFd = Open("/dev/stdout", OFlags::WRONLY);
}


OStream::~OStream() {
    if (!isClosed) {
        close();
    }
    delete data;
}

SSize OStream::write(ConstPtr buffer, Size bufferSize) {
    return (this->*writeFunc)(buffer, bufferSize);
}

int OStream::close() {
    if (isClosed) return -1;
    return (this->*closeFunc)();
}


SSize OStream::writeToStdout(ConstPtr buffer, Size bufferSize) {
    return Write(data->stdoutFd, buffer, bufferSize);
}


#ifdef __simbuild__

SSize OStream::writeToFile(ConstPtr buffer, Size buffer_size) {
    return FWrite(data->file, buffer, buffer_size);
}

#endif //__simbuild__


int OStream::closeStdout() {
    return Close(data->stdoutFd);
}


#ifdef __simbuild__

int OStream::closeFile() {
    return FClose(data->file);
}

#endif //__simbuild__

