/**
 * Created by Ilya Potemin on 11/7/18.
 * Part of Phantom Shell project.
 */

#ifndef PHANTOMSHELL_STRING_H
#define PHANTOMSHELL_STRING_H

#include "alloc.h"

namespace phlib {

class string {

private:
    const char *value;
    unsigned int length;

public:

    string() {

    }

    ~string() {

    }

    string &operator+=(const string &other) {
        auto *new_str = new string();
        return *new_str;
    }

    void *operator new(unsigned long size) {
        void *pointer = phlib::malloc(size);
        return pointer;
    }

    void operator delete(void *pointer) {
        phlib::free(pointer);
    }

};

}
#endif //PHANTOMSHELL_STRING_H
