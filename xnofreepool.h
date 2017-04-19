// xnofreepool
// 这个pool托管了void *buf, 并维护一个指针,每次分配,指针都往后挪对应的大小,然后把这块内存给用户
// 当内存free的时候,并不回收内存, 也就是那个指针 只能 一直往后挪, 直到buffer分配光
// 有clear方法,调用clear之后,指针=0,然后从头开始分配.(备注:实际是从尾巴开始分配)

#ifndef CHECKKING_XNOFREEPOOL_H
#define CHECKKING_XNOFREEPOOL_H
namespace checkking {
namespace mempool {
class xnofreepool : public mempool
{
public:
	char * _buffer;
	size_t _bufcap;
	size_t _bufleft;

	size_t _free;   // 空闲且不可分配的内存空间大小
public:
	inline void create (void *buf, size_t bufsiz) {
		_buffer = (char *)buf;
		_bufcap = bufsiz;
		_bufleft = bufsiz;
		_free = 0;
	}
	inline void * malloc (size_t size) {
		if (size > _bufleft) return NULL;
		_bufleft -= size;
		return _buffer + _bufleft;
	}
	inline void free (void *, size_t size) {
		_free += size;
#ifdef NOFREEPOOL_AUTO_CLEAR
		if (_free == _bufcap) {
			clear();
		}
#endif
	}
	inline void clear () {
		_bufleft = _bufcap;
		_free = 0;
	}
	void destroy () {
		_buffer = NULL;
		_bufcap = 0;
		_bufleft = 0;
		_free = 0;
	}
};
}
}
#endif
