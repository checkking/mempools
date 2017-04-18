#include "cachedpoolappend.h"

namespace checkking {
namespace mempool {
void * cached_mempool_append :: malloc(size_t size)
{
    if (size <= 0 || NULL == _head) {
        return 0;
    }

    if (_buff_freesize >= size) {
        void *p = _free_space;
        _buff_freesize -= size;
        _free_space += size;
        return p;
    } 
    if (size <= _seg_size - sizeof(void *)) {
        while (_seg_usingnum < _seg_num) {
            if(size > _buff_freesize) {
                ++_seg_usingnum;
                _pre_seg = _curr_seg;
                _curr_seg = *(void **)_curr_seg;
                _buff_freesize = _seg_size - sizeof(void *);
                _free_space = (char *)_curr_seg + sizeof(void *);
            } else {
                void * p = _free_space;
                _buff_freesize -= size;
                _free_space += size;
                return p;
            }
        }
        void *tmp = ::malloc(_seg_size);
        if(tmp) {
            *(void **)tmp = NULL;
            *(void **)_pre_seg = tmp;
            _curr_seg = tmp;
            ++_seg_num;
            _seg_usingnum = _seg_num - 1;

            _free_space = (char *)tmp + sizeof(void *) + size;
            _buff_freesize = _seg_size - sizeof(void *) - size;
            void *p = (char *)tmp + sizeof(void *);
            return p;
        } else {
            return 0;
        }
    } else {
        void *tmp = ::malloc(size + sizeof(void *));
        if(tmp)
        {
            *(void **)tmp = _bigbuf_head;
            _bigbuf_head = tmp;
            return ((char *)tmp + sizeof(void *));
        }
        else {
            return 0;
        }
    }
}

} // namespace mempool
} // namespace checkking
