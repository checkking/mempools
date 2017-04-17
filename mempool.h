#ifndef CHECKKING_MEMPOOL_H
#define CHECKKING_MEMPOOL_H
namespace checkking {
namespace mempool {
class mempool {
public:
    virtual void* malloc(size_t size) = 0;
    virtual void free(void *p, size_t size) = 0;
    inline virtual ~mempool() {
    }
};

class syspool : public mempool {
public:
    inline virtual void* malloc(size_t size) {
        return ::malloc(size);
    }
    inline virtual void free(void* p, size_t) {
        ::free(p);
    }
};
}
}
#endif // CHECKKING_MEMPOOL_H
