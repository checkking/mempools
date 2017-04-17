#ifndef CHECKKING_CACHEDPOOL_H
#define CHECKKING_CACHEDPOOL_H

#include "mempool.h"

namespace checkking {
namespace mempool {
class cachedpool : public mempool {
public:
    inline cachedpool: 
            _buf_size(0),
            _free_space(0),
            _memlist(0) () {
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

    virtual inline cachedpool() {
        while (_memlist) {
            void* tmp = *(void **)_memlist;
            ::free(_memlist);
            _memlist = tmp;
        }
    }

private:
    size_t _buf_size;
    char* _free_space;
    void* _memlist;

    static const int _s_seg_size = 4096;
}
}
}
#endif // CHECKKING_CACHEDPOOL_H
