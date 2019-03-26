/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "OStream.h"

//@formatter:off
union phlib::OStream::OStreamData {
#ifdef __simbuild__
    /** Stores output file descriptor, when type==FILE_STREAM */
    File *file;
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


phlib::OStream::OStream(String &filePath) {
    data = new OStreamData();
    type = OStreamType::FILE_STREAM;
    writeFunc = &OStream::writeToFile;
    closeFunc = &OStream::closeFile;
    data->file = fopen(filePath, "w+");
}


phlib::OStream::OStream(Ptr buffer) {
    //TODO: implement later
}


phlib::OStream::OStream() {
    data = new OStreamData();
    type = OStreamType::STDOUT_STREAM;
    writeFunc = &OStream::writeToStdout;
    closeFunc = &OStream::closeStdout;
    data->stdoutFd = open("/dev/stdout", OFlags::WRONLY);
}


phlib::OStream::~OStream() {
    if (!isClosed) {
        close();
    }
    delete data;
}

SSize phlib::OStream::write(ConstPtr buffer, Size bufferSize) {
    return (this->*writeFunc)(buffer, bufferSize);
}

int phlib::OStream::close() {
    if (isClosed) return -1;
    return (this->*closeFunc)();
}


SSize phlib::OStream::writeToStdout(ConstPtr buffer, Size bufferSize) {
    return phlib::write(data->stdoutFd, buffer, bufferSize);
}


#ifdef __simbuild__

SSize phlib::OStream::writeToFile(ConstPtr buffer, Size buffer_size) {
    return phlib::fwrite(data->file, buffer, buffer_size);
}

#endif //__simbuild__


int phlib::OStream::closeStdout() {
    phlib::close(data->stdoutFd);
}


#ifdef __simbuild__

int phlib::OStream::closeFile() {
    phlib::fclose(data->file);
}

#endif //__simbuild__

