#include "cachedpool.h"
#include <iostream>
#include <stdio.h>

int main(int argc, char** argv) {
    using namespace checkking::mempool;

    cachedpool pool;

    char* buf = (char *)pool.malloc(100);
    snprintf(buf, 100, "%s", "hello world");
    std::cout << buf << std::endl;

    return 0;
}
