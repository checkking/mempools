#include "xcompool.h"
#include <iostream>
#include <stdio.h>

int main(int argc, char** argv) {
    using namespace checkking::mempool;

    xcompool pool;
    pool.create(1024);

    char* buf = (char *)pool.malloc(100);
    snprintf(buf, 100, "%s", "hello world");
    std::cout << buf << std::endl;
    pool.free(buf, 100);
    pool.destroy();
    
    buf = (char *)pool.malloc(4096);
    snprintf(buf, 4096, "%s", "Big Hello World!");
    std::cout << buf << std::endl;
    pool.free(buf, 100);
    pool.destroy();
    return 0;
}
