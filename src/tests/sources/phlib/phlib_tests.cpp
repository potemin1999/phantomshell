#include "Types.h"
#include "Phlib.h"

using namespace phlib;

int test1(){
    Byte srcBytePtr[10];
    Byte dstBytePtr[10];
    srcBytePtr[0] = 0;
    srcBytePtr[1] = 1;
    srcBytePtr[2] = 2;
    srcBytePtr[3] = 3;
    srcBytePtr[4] = 4;
    srcBytePtr[5] = 5;
    srcBytePtr[6] = 6;
    srcBytePtr[7] = 7;
    srcBytePtr[8] = 8;
    srcBytePtr[9] = 9;
    memcpy(dstBytePtr,srcBytePtr,10);
    return 0;
}

int main(int argc, const char **argv) {
    int ret = 0;
    if ((ret = test1())) return ret;
    return 0;
}

