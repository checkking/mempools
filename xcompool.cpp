#include "xcompool.h"

namespace checkking {
namespace mempool {
int xcompool::create(size_t maxblk, size_t bmin, size_t bmax, float rate)
{
    destroy();
    _buffer = ::malloc (maxblk);
    _bufsiz = maxblk;
    if (_buffer == NULL) return -1;
    int ret = _pool.create(_buffer, maxblk, bmin, bmax, rate);
    if (ret != 0) {
        destroy();
        return -1;
    }
    _maxblksiz = maxblk;
    if (_maxblksiz < _pool.max_alloc_size()) {
        _maxblksiz = _pool.max_alloc_size();
    }

    return ret;
}

void xcompool::destroy()
{
    _pool.destroy();
    for (SET::iterator iter = _set.begin(); iter != _set.end(); ++iter) {
        ::free (*iter);
    }
    _set.clear();
    if (_buffer) {
        ::free (_buffer);
    }
    _buffer = 0;
    _bufsiz = 0;
    _maxblksiz = 0;
}

void * xcompool::malloc (size_t size)
{
    void * ret = NULL;
    if (size > _pool.max_alloc_size()) {
        ret = ::malloc(size);
        if (ret) {
            _set.insert(ret);
        }
        return ret;
    }
    ret = _pool.malloc(size);
    if (ret) return ret;
    ret = ::malloc (_maxblksiz);
    if (ret == NULL) return NULL;
    _set.insert(ret);
    _pool.addbuf(ret, _maxblksiz);
    return _pool.malloc(size);
}

void xcompool::free (void *ptr, size_t size)
{
    if (size > _pool.max_alloc_size()) {
        ::free(ptr);
        _set.erase(ptr);
        return;
    }
    _pool.free(ptr, size);
}

void xcompool::clear ()
{
    _pool.clear(_buffer, _bufsiz);
    for (SET::iterator iter=_set.begin(); iter != _set.end(); ++iter) {
        ::free (*iter);
    }
    _set.clear();
}
}
}
