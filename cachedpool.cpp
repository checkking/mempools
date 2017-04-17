#include "cachedpool.h"

namespace checkking {
namespace mempool {
void* cachedpool::malloc(size_t size) {
    size = (size + 7 ) & ( -1 << 3); // alignment, -1(0xffff)
    if (size < _s_seg_size - sizeof(void *)) {
        if (size > _buf_size) {
            void* tmp = ::malloc(_s_seg_size);
            if (tmp) {
                *(void **)tmp = _memlist;
                _memlist = tmp;
                _free_space = (char *)tmp + sizeof(void *);
                _buf_size = _s_seg_size - sizeof(void *);
            } else {
                return 0;
            }
        }
        _buf_size -= size;
        void * p = _free_space;
        _free_space += size;
        return p;
    } else {
        void *tmp = ::malloc(size + sizeof(void *));
        if(tmp) {
            *(void **)tmp = _memlist;
            _memlist = tmp;
            return ((char *)tmp + sizeof(void *));
        } else {
            return 0;
        }
    }
}
}
}
