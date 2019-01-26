//#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace phlib;


int test1() {
    auto str = new String();
    *str += "hello";
    *str += " world";
    return String::strcmp(str->char_value(), "hello world");
}


int main(int argc, const char **argv) {
    int ret = 0;
    if ((ret = test1())) return ret;
    return 0;
}