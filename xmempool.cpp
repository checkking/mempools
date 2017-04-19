#include "xmempool.h"

namespace checkking {
namespace mempool {
#define DELSIZE(rsize, size) \
    size_t rsize = (((size) + sizeof(int) - 1) / sizeof(int) * sizeof(int))

#define RSIZE(rsize, size) \
    rsize = (((size) + sizeof(int) - 1) / sizeof(int) * sizeof(int))

xmempool::xmempool()
{
    _maxsiz = 0;
    _pool = 0;
    _rate = 0;
}

xmempool::~xmempool()
{
    destroy();
}

int xmempool::create(void *buf, size_t bufsiz,
        size_t bmin, size_t bmax, float rate) {
    DELSIZE(__bmin, bmin);
    DELSIZE(__bmax, bmax);
    if (__bmin < sizeof(void *)) __bmin = sizeof(void *);
    if (__bmin > __bmax) return -1;

    if (buf == NULL || bufsiz == 0 || rate < 1.01f) return -1;

    size_t __b = __bmin;
    int lvl = 1;
    while (__b < __bmax) {
        ++ lvl;
        RSIZE(__b, (size_t)(__b * rate));
    }

    _mlc.create(buf, bufsiz);
    _poolsize = lvl;
    _minsiz = __bmin;
    _rate = rate;
    return clear();
}

int xmempool::clear()
{
    return clear (_mlc._buffer, _mlc._bufcap);
}

int xmempool::clear (void *buffer, size_t size)
{
    _mlc.create (buffer, size);
    _pool = (xfixmemg *)_mlc.malloc(sizeof(xfixmemg) * _poolsize);
    if (_pool == NULL) return -1;

    _maxsiz = 0;
    size_t m = _minsiz;
    for (int i=0; i<_poolsize; ++i) {
        _pool[i].create(m);
        _maxsiz = m;
        RSIZE(m, (size_t)(m * _rate));
    }
    return 0;
}


// 并没有释放托管的内存空间，只是将所有变量置0或者NULL，
// 托管的内存空间需要在外部释放 
int xmempool::destroy()
{
    _mlc.destroy();
    _pool = 0;
    _maxsiz = 0;
    _minsiz = 0;
    _poolsize = 0;
    _rate = 0;
    return 0;
}

int xmempool::getidx(size_t size)
{
    if (size > _pool[_poolsize-1]._size) return -1;
    int len = _poolsize, half=0, mid=0, first=0;

    while (len > 0) {
        half = len >> 1;
        mid = first + half;
        if (_pool[mid]._size < size) {
            first = mid + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

// 将以前剩下的托管内存进行回收，添加新的托管内存。
void * xmempool::addbuf (void *buf, size_t size)
{
    void * oldbuf = _mlc._buffer;
    for (int i=_poolsize-1; i>=0; --i) {
        void * ret = NULL;
        while ( (ret = _mlc.malloc(_pool[i]._size)) != NULL) {
            _pool[i].free (ret);
        }
    }
    _mlc.create (buf, size);
    return oldbuf;
}    
} // namespace mempool
} // namespace checkking
