// xfixmemg
// xifixmemg用于xmempool的内存回收
//

#ifndef CHECKKING_XFIXMEMG_H
#define CHECKKING_XFIXMEMG_H

namespace checkking {
namespace mempool {
union xmem_node_t
{
	xmem_node_t *next;
	char data[0];
};

class xfixmemg
{
public:
	xmem_node_t *_freelst;
	size_t _size;
public:
	inline xfixmemg() { create(0); }
	inline ~xfixmemg() { create(0); }
	inline void create (int size) {
		_size = size;
		_freelst = 0;
	}
	inline void destroy () {
		create (0);
	}
	inline void * malloc () {
		if (_freelst) {
			xmem_node_t *node = _freelst;
			_freelst = _freelst->next;
			return (void *)node->data;
		}
		return NULL;
	}

	inline void free(void *ptr) {
		((xmem_node_t *)ptr)->next = _freelst;
		_freelst = (xmem_node_t *) ptr;
	}
};

}
}

#endif
