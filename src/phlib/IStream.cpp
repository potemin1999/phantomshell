/**
 * @file
 * @author Ilya Potemin <i.potemin@innopolis.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "IStream.h"

using namespace phlib;

//@formatter:off
union IStream::IStreamData {
#ifdef __simbuild__
    /** Stores input file descriptor, when type==FILE_STREAM*/
    File file;
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


IStream::IStream(String &filePath) {
    data = new IStreamData();
    type = IStreamType::FILE_STREAM;
    readFunc = &IStream::readFromFile;
    closeFunc = &IStream::closeFile;
    data->file = FOpen(filePath, "r");
}


IStream::IStream(ConstPtr byteBuffer, Size size, bool doCopy) {
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


IStream::IStream() {
    data = new IStreamData();
    type = IStreamType::STDIN_STREAM;
    readFunc = &IStream::readFromStdin;
    closeFunc = &IStream::closeStdin;
    data->stdinFd = Open("/dev/stdin", OFlags::RDONLY);
}


IStream::~IStream() {
    if (!isClosed) {
        close();
    }
    delete data;
}

SSize IStream::read(Ptr buffer, Size bufferSize) {
    if (bufferSize < 0) return -1;
    if (buffer == nullptr && bufferSize > 0) return -2;
    return (this->*readFunc)(buffer, bufferSize);
}


int IStream::close() {
    if (isClosed) return -1;
    return (this->*closeFunc)();
}


#ifdef __simbuild__

SSize IStream::readFromFile(Ptr buffer, Size bufferSize) {
    return FRead(data->file, buffer, bufferSize);
}

#endif //__simbuild__


SSize IStream::readFromStdin(Ptr buffer, Size bufferSize) {
    return Read(data->stdinFd, buffer, bufferSize);
}


SSize IStream::readFromObject(Ptr buffer, Size bufferSize) {
    auto read_size = bufferSize < (data->objectSize - data->currentPointer - 1) ?
                     bufferSize : data->objectSize - data->currentPointer - 1;
    auto byteBuffer = static_cast<Byte *>(buffer);
    auto objectBytes = static_cast<const Byte *>(data->object);
    for (int i = 0; i < read_size; i++, data->currentPointer++) {
        byteBuffer[i] = objectBytes[data->currentPointer];
    }
    return read_size;
}


int IStream::closeObject() {
    if (data->objectCopy) {
        Free(const_cast<Ptr>(data->object));
    }
    return 0;
}


int IStream::closeStdin() {
    return Close(data->stdinFd);
}


#ifdef __simbuild__

int IStream::closeFile() {
    return FClose(data->file);
}

#endif //__simbuild__
