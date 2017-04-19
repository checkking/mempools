// cachedpool
// 这是一个内存缓存器，他从系统分配大块内存，
// 切出应用程序需要的大小，以提高内存分配性能。不能回收内存，
// 调用clear方法后清空大部分内存， 但会保留一块固定大小的内存供下次分配使用。
// 因此，一个cached_mempool可以在clear之后重复使用，以提高性能。 
// 在cached_mempool析构时，会释放所分配的所有内存
//
// 优点：
// 1) 小对象的快速分配
// 2) 避免了内存碎片的生成
//
// 缺点：
// 1) 内存没有回收利用
//
// 适用于内存与请求绑定的场景，尤其是每次请求需要的内存相差比较大的场景
// 不适用于长时间持有内存的场景

#ifndef CHECKKING_CACHEDPOOL_H
#define CHECKKING_CACHEDPOOL_H

#include "mempool.h"

namespace checkking {
namespace mempool {
class cachedpool : public mempool {
public:

    inline cachedpool() {
        _buf_size = 0;
        _free_size = 0;
        _free_space = 0;
        _memlist = 0;
    }

    virtual void* malloc(size_t size);

    virtual inline void free(void*, size_t) {
        // unimplemented.
    }

    inline void clear() {
        if (_memlist == 0) {
            return;
        }
        while (*(void **)_memlist) { // if next bulk is not null
            void *tmp = *(void **)_memlist;
            ::free(_memlist);
            _memlist = tmp;    // point to next bulk
        }

        // we keep a bulk for feature use.
        _free_space = (char*)_memlist + sizeof(void*);
        _buf_size = _s_seg_size - sizeof(void*);
    }

    virtual inline ~cachedpool() {
        while (_memlist) {
            void* tmp = *(void **)_memlist;
            ::free(_memlist);
            _memlist = tmp;
        }
    }

private:
    size_t _buf_size;
    size_t _free_size;
    char* _free_space;
    void* _memlist;

    static const int _s_seg_size = 4096;
}; // class cachedpool
}
}
#endif // CHECKKING_CACHEDPOOL_H
