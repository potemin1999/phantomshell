/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "ostream.h"


union phlib::OStream::OStreamData {
#ifdef __simbuild__
    /** Stores output file descriptor, when type==FILE_STREAM */
    File *file;
#endif //__simbuild__
    struct {
        /** Stores pointer to destination object, when type==OBJECT_STREAM*/
        Ptr object;
        bool object_copy;
        SSize current_pointer;
        /** Stores destination object size, when type==OBJECT_STREAM*/
        SSize object_size;
    };
    /** Stores stdout descriptor, when type==STDOUT_STREAM*/
    int stdout_fd;

    Ptr operator new(Size size) {
        return Allocator::get_default_allocator()->allocate(size);
    }

    void operator delete(Ptr ptr) {
        Allocator::get_default_allocator()->deallocate(ptr);
    }
};


phlib::OStream::OStream(String &file_path) {
    data = new OStreamData();
    type = OStreamType::FILE_STREAM;
    write_func = &OStream::write_to_file;
    close_func = &OStream::close_file;
    data->file = fopen(file_path, "w+");
}


phlib::OStream::OStream(Ptr buffer) {
    //TODO: implement later
}


phlib::OStream::OStream() {
    data = new OStreamData();
    type = OStreamType::STDOUT_STREAM;
    write_func = &OStream::write_to_stdout;
    close_func = &OStream::close_stdout;
    data->stdout_fd = open("/dev/stdout", OFlags::WRONLY);
    DEBUG_LOG("created new stdout output stream with stdout_fd = %d\n", data->stdout_fd);
}


phlib::OStream::~OStream() {
    if (!isClosed) {
        close();
    }
    delete data;
}

Ptr phlib::OStream::operator new(Size size) {
    return Allocator::get_default_allocator()->allocate(size);
}

void phlib::OStream::operator delete(Ptr pointer) {
    Allocator::get_default_allocator()->deallocate(pointer);
}

SSize phlib::OStream::write(ConstPtr buffer, Size buffer_size) {
    return (this->*write_func)(buffer, buffer_size);
}

int phlib::OStream::close() {
    if (isClosed) return -1;
    return (this->*close_func)();
}


SSize phlib::OStream::write_to_stdout(ConstPtr buffer, Size buffer_size) {
    return phlib::write(data->stdout_fd, buffer, buffer_size);
}


#ifdef __simbuild__

SSize phlib::OStream::write_to_file(ConstPtr buffer, Size buffer_size) {
    return phlib::fwrite(data->file, buffer, buffer_size);
}

#endif //__simbuild__


int phlib::OStream::close_stdout() {
    phlib::close(data->stdout_fd);
}


#ifdef __simbuild__

int phlib::OStream::close_file() {
    phlib::fclose(data->file);
}

#endif //__simbuild__

