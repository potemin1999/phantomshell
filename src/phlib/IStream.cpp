/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "IStream.h"

//@formatter:off
union phlib::IStream::IStreamData {
#ifdef __simbuild__
    /** Stores input file descriptor, when type==FILE_STREAM*/
    File *file;
#endif //__simbuild__
    struct {
        /** Stores pointer to source object, when type==OBJECT_STREAM*/
        Ptr   object;
        bool  objectCopy;
        SSize currentPointer;
        /** Stores source object size, when type==OBJECT_STREAM*/
        SSize objectSize;
    };
    /** Stores stdin descriptor, when type==STDIN_STREAM*/
    int  stdinFd;
};
//@formatter:on


phlib::IStream::IStream(String &filePath) {
    data = new IStreamData();
    type = IStreamType::FILE_STREAM;
    readFunc = &IStream::readFromFile;
    closeFunc = &IStream::closeFile;
    data->file = phlib::fopen(filePath, "r");
}


phlib::IStream::IStream(ConstPtr byteBuffer, Size size, bool doCopy) {
    data = new IStreamData();
    type = IStreamType::OBJECT_STREAM;
    readFunc = &IStream::readFromObject;
    closeFunc = &IStream::closeObject;
    if (doCopy) {
        data->object = phlib::malloc(size);
        for (int i = 0; i < size; ((char *) data->object)[i] = ((char *) byteBuffer)[i], i++);
    } else {
        data->object = (Ptr) byteBuffer;
    }

    data->objectCopy = doCopy;
    data->objectSize = size;
    data->currentPointer = 0;
}


phlib::IStream::IStream() {
    data = new IStreamData();
    type = IStreamType::STDIN_STREAM;
    readFunc = &IStream::readFromStdin;
    closeFunc = &IStream::closeStdin;
    data->stdinFd = phlib::open("/dev/stdin", OFlags::RDONLY);
    DEBUG_LOG("created input stream from stdin with stdinFd = %d\n", data->stdinFd);
}


phlib::IStream::~IStream() {
    if (!isClosed) {
        close();
    }
    delete data;
}

SSize phlib::IStream::read(Ptr buffer, Size bufferSize) {
    if (bufferSize < 0) return -1;
    if (buffer == nullptr && bufferSize > 0) return -2;
    return (this->*readFunc)(buffer, bufferSize);
}


int phlib::IStream::close() {
    if (isClosed) return -1;
    return (this->*closeFunc)();
}


#ifdef __simbuild__

SSize phlib::IStream::readFromFile(Ptr buffer, Size bufferSize) {
    return phlib::fread(data->file, buffer, bufferSize);
}

#endif //__simbuild__


SSize phlib::IStream::readFromStdin(Ptr buffer, Size bufferSize) {
    return phlib::read(data->stdinFd, buffer, bufferSize);
}


SSize phlib::IStream::readFromObject(Ptr buffer, Size bufferSize) {
    ssize_t read_size = bufferSize < (data->objectSize - data->currentPointer - 1) ?
                        bufferSize : data->objectSize - data->currentPointer - 1;

    for (int i = 0; i < read_size; i++, data->currentPointer++) {
        ((char *) buffer)[i] = ((char *) data->object)[data->currentPointer];
    }
    return read_size;
}


int phlib::IStream::closeObject() {
    if (data->objectCopy) {
        phlib::free(data->object);
    }
}


int phlib::IStream::closeStdin() {
    phlib::close(data->stdinFd);
}


#ifdef __simbuild__

int phlib::IStream::closeFile() {
    phlib::fclose(data->file);
}

#endif //__simbuild__
