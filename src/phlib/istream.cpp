/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/27/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include <phlib/istream.h>


union phlib::IStream::IStreamData {
#ifdef __simbuild__
    /** Stores input file descriptor, when type==FILE_STREAM*/
    File *file;
#endif //__simbuild__
    struct {
        /** Stores pointer to source object, when type==OBJECT_STREAM*/
        Ptr object;
        bool object_copy;
        SSize current_pointer;
        /** Stores source object size, when type==OBJECT_STREAM*/
        SSize object_size;
    };
    /** Stores stdin descriptor, when type==STDIN_STREAM*/
    int stdin_fd;

    Ptr operator new(Size size) {
        return Allocator::get_default_allocator()->allocate(sizeof(IStreamData));
    }

    void operator delete(Ptr ptr) {
        Allocator::get_default_allocator()->deallocate(ptr);
    }

};


phlib::IStream::IStream(String &file_path) {
    data = new IStreamData();
    type = IStreamType::FILE_STREAM;
    read_func = &IStream::read_from_file;
    close_func = &IStream::close_file;
    data->file = phlib::fopen(file_path, "r");
}


phlib::IStream::IStream(ConstPtr byte_buffer, Size size, bool do_copy) {
    data = new IStreamData();
    type = IStreamType::OBJECT_STREAM;
    read_func = &IStream::read_from_object;
    close_func = &IStream::close_object;
    if (do_copy) {
        data->object = phlib::malloc(size);
        for (int i = 0; i < size; ((char *) data->object)[i] = ((char *) byte_buffer)[i], i++);
    } else {
        data->object = (Ptr) byte_buffer;
    }
    data->object_copy = do_copy;
    data->object_size = size;
    data->current_pointer = 0;
}


phlib::IStream::IStream() {
    data = new IStreamData();
    type = IStreamType::STDIN_STREAM;
    read_func = &IStream::read_from_stdin;
    close_func = &IStream::close_stdin;
    data->stdin_fd = phlib::open("/dev/stdin", OFlags::RDONLY);
    DEBUG_LOG("created input stream from stdin with stdin_fd = %d\n", data->stdin_fd);
}


phlib::IStream::~IStream() {
    if (!isClosed) {
        close();
    }
    delete data;
}


Ptr phlib::IStream::operator new(Size size) {
    return Allocator::get_default_allocator()->allocate(size);
}


void phlib::IStream::operator delete(Ptr pointer) {
    Allocator::get_default_allocator()->deallocate(pointer);
}


SSize phlib::IStream::read(Ptr buffer, Size buffer_size) {
    if (buffer_size < 0) return -1;
    if (buffer == nullptr && buffer_size > 0) return -2;
    return (this->*read_func)(buffer, buffer_size);
}


int phlib::IStream::close() {
    if (isClosed) return -1;
    return (this->*close_func)();
}


#ifdef __simbuild__

SSize phlib::IStream::read_from_file(Ptr buffer, Size buffer_size) {
    return phlib::fread(data->file, buffer, buffer_size);
}

#endif //__simbuild__


SSize phlib::IStream::read_from_stdin(Ptr buffer, Size buffer_size) {
    return phlib::read(data->stdin_fd, buffer, buffer_size);
}


SSize phlib::IStream::read_from_object(Ptr buffer, Size buffer_size) {
    ssize_t read_size = buffer_size < (data->object_size - data->current_pointer - 1) ?
                        buffer_size : data->object_size - data->current_pointer - 1;
    for (int i = 0; i < read_size; i++, data->current_pointer++) {
        ((char *) buffer)[i] = ((char *) data->object)[data->current_pointer];
    }
    return read_size;
}


int phlib::IStream::close_object() {
    if (data->object_copy) {
        phlib::free(data->object);
    }
}


int phlib::IStream::close_stdin() {
    phlib::close(data->stdin_fd);
}


#ifdef __simbuild__

int phlib::IStream::close_file() {
    phlib::fclose(data->file);
}

#endif //__simbuild__
