#include "InputStream.h"
#include <stdio.h>

using namespace phlib;


int test1() {
    const int size = 500;
    char *src_buffer = (char *) phlib::malloc(size);
    for (int i = 0; i < size; i++) {
        src_buffer[i] = 'a' + (char) (i % ('z' - 'a' + 1));
    }
    src_buffer[size - 1] = '\0';
    auto input_stream = new IStream(src_buffer, size, false);
    char buffer[2];
    buffer[1] = '\0';
    for (int j = 0; j < size; ++j) {
        input_stream->read(&buffer, 1);
        printf("%s", buffer);
    }
    return 0;
}


int main(int argc, const char **argv) {
    int ret = 0;
    if ((ret = test1())) return ret;
    return 0;
}

