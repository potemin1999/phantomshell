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
        ConstPtr object;
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
        auto charObject = new Byte[size];
        auto charBuffer = static_cast<const Byte *>(byteBuffer);
        for (int i = 0; i < size; charObject[i] = charBuffer[i], i++);
        data->object = charObject;
    } else {
        data->object = byteBuffer;
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
    auto byteBuffer = static_cast<Byte *>(buffer);
    auto objectBytes = static_cast<const Byte *>(data->object);
    for (int i = 0; i < read_size; i++, data->currentPointer++) {
        byteBuffer[i] = objectBytes[data->currentPointer];
    }
    return read_size;
}


int phlib::IStream::closeObject() {
    if (data->objectCopy) {
        phlib::free(const_cast<Ptr>(data->object));
    }
    return 0;
}


int phlib::IStream::closeStdin() {
    return phlib::close(data->stdinFd);
}


#ifdef __simbuild__

int phlib::IStream::closeFile() {
    return phlib::fclose(data->file);
}

#endif //__simbuild__
