#include "cachedpoolappend.h"
#include <iostream>
#include <stdio.h>

int main(int argc, char** argv) {
    using namespace checkking::mempool;
    cached_mempool_append pool;
    char* buffer = (char*)malloc(512);
    pool.create(buffer,512,1024);
    char* buf = (char*)pool.malloc(100);
    sprintf(buf,"%s","hello world");
    std::cout << buf << std::endl;
    pool.clear();
    buf = (char*)pool.malloc(5096);
    sprintf(buf,"%s","com");
    std::cout << buf << std::endl;
    pool.destroy();
    // error!
    // ::free(buf);
    ::free(buffer);

    pool.create(512,1024);
    // 申请100大小的字符串数组
    buf = (char*)pool.malloc(100); 
    sprintf(buf,"%s","xxxxx");
    std::cout << buf << std::endl;
    
    return 0;
}
