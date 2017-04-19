// xmempool
// 托管一块内存，每次从托管的内存中切出一块用于分配，
// 如果内存不够将返回NULL，能够回收并重复利用内存。
//  
//  优点：
// 能够回收内存，提高内存利用率。
// 能够指定内存增长率，能够准确的回收相应大小的内存块。
// 
// 缺点：
// 当内存不够时，内存空间无法动态增长。
// xmempool中实际能够使用的内存空间
// 并没有传入的托管内存那么大
// 因为其本身的一些初始化也需要用到这些内存

#ifndef CHECKKING_XMEMPOOL_H
#define CHECKKING_XMEMPOOL_H

#include "xfixmemg.h"
#include "xnofreepool.h"

namespace checkking {
namespace mempool {
class xmempool : public mempool {
public:
    xmempool ();
    ~xmempool ();
    int create (void * buf, size_t bufsiz, 
            size_t bmin = sizeof(void *), size_t bmax = (1<<20), float rate = 2.0f);
    int clear();
    int clear (void *buffer, size_t size);
    int destroy ();

    void * addbuf (void *buf, size_t size);
    
    inline void * malloc (size_t size) {
        int idx = getidx (size);
        if (idx >= 0) {
            void * ret = _pool[idx].malloc();
            if (ret) {
                return ret;
            }
            return  _mlc.malloc (_pool[idx]._size);
        }
        return NULL;
    }
    
    inline void free (void *ptr, size_t size) {
        if (ptr == NULL) {
            return;
        }
        int idx = getidx(size);
        if (idx >= 0) {
            _pool[idx].free(ptr);
        }
    }
    
    size_t goodsize (size_t size) {
        int idx =  getidx(size);
        if (idx < 0 || idx >= _poolsize) { 
            return 0;
        }
        return _pool[idx]._size;
    }

    inline size_t max_alloc_size() {
        return _maxsiz;
    }

private:
    int getidx (size_t size);
    static const int CNT = 10;

    xfixmemg *_pool;    //实际内存管理器
    int _poolsize;
    xnofreepool _mlc; //实际内存分配器
    size_t _minsiz;
    size_t _maxsiz;    //最大可分配内存
    float _rate;
}; // class xmempool
}
}
#endif
