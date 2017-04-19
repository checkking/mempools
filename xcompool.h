// xcompool
// 与xmempool相比，自身维护一块buffer，当内存不够时，能够动态扩展。 
// 
// 适用场景:
// 如果是一些单进程程序，可以考虑.
// 由于分配的时候不需要加锁解锁， 性能有一定的保证
// 适用于单进程程序长时间占用内存的场景

#ifndef CHECKKING_XCOMPOOL_H
#define CHECKKING_XCOMPOOL_H

#include <set>
#include "mempool.h"
#include "xmempool.h"

namespace checkking {
namespace mempool {
class xcompool : public mempool {
public:
    xcompool() {
        _maxblksiz = 0;
        _buffer = NULL;
    }
    ~xcompool() {
        destroy();
    }
    int create(size_t maxblk = 1<<20, 
        size_t bmin = sizeof(void *), size_t bmax = (1<<16), 
        float rate = 2.0f);
    void destroy();
    
    void * malloc(size_t size);
    
    void free(void *ptr, size_t size);
    
    void clear();

private:
    size_t _maxblksiz;
    void * _buffer;
    size_t _bufsiz;
    xmempool _pool;
    typedef std::set<void *> SET;
    // 当需要的内存大小无法使用_buffer分配时，
    // 直接调用malloc，_set保存malloc返回的首地址。
    SET _set; 
}; // class xcompool
}
}
#endif
