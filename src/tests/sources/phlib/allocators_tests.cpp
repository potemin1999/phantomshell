/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 11/28/18
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "Alloc.h"
#include <chrono>
#include <stdio.h>


using namespace std::chrono;


#define TEST_1_SIZE 100000


phlib::Allocator *current_allocator;


class MallocTestStruct {
public:
    char a = 'a';
    int *b = nullptr;
    char *c = nullptr;
    long long d = 11;

    Ptr operator new(Size size) {
        return phlib::malloc(size);
    }

    void operator delete(Ptr ptr) {
        phlib::free(ptr);
    }
};


class AllocatorTestStruct {
public:
    char a = 'a';
    int *b = nullptr;
    char *c = nullptr;
    long long d = 11;

    Ptr operator new(Size size) {
        return current_allocator->allocate(size);
    }

    void operator delete(Ptr ptr) {
        current_allocator->deallocate(ptr);
    }
};


int test1_malloc() {
    MallocTestStruct *ptr[TEST_1_SIZE];
    for (auto &i : ptr) {
        i = new MallocTestStruct();
        i->a++;
        i->d++;
    }
    for (auto &i : ptr) {
        delete i;
    }
    return 0;
}


int test1_allocator() {
    current_allocator = phlib::Allocator::getDefaultAllocator();
    AllocatorTestStruct *ptr[TEST_1_SIZE];
    for (auto &i : ptr) {
        i = new AllocatorTestStruct();
        i->a++;
        i->d++;
    }
    for (auto &i : ptr) {
        delete i;
    }
    return 0;
}


int test1() {
    high_resolution_clock::time_point start_time1 = high_resolution_clock::now();
    test1_malloc();
    high_resolution_clock::time_point end_time1 = high_resolution_clock::now();
    duration<double> time_span1 = duration_cast<duration<double>>(end_time1 - start_time1);
    high_resolution_clock::time_point start_time2 = high_resolution_clock::now();
    test1_allocator();
    high_resolution_clock::time_point end_time2 = high_resolution_clock::now();
    duration<double> time_span2 = duration_cast<duration<double>>(end_time2 - start_time2);
    printf("malloc sequential allocation took %f seconds\n", time_span1.count());
    printf("allocator sequential allocation took %f seconds\n", time_span2.count());
    printf("allocator/malloc performance : %f", (time_span1.count() / time_span2.count()));
    return 0;
}


int main(int argc, const char **argv) {
    int ret = 0;
    if ((ret = test1())) return ret;
    return 0;
}