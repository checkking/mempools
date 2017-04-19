### syspool
这个pool没有clear方法，分配的时候，直接调用malloc和free

### cachedpool
这是一个内存缓存器，他从系统分配大块内存，切出应用程序需要的大小，以提高内存分配性能。不能回收内存，调用clear方法后清空大部分内存， 但会保留一块固定大小的内存供下次分配使用。因此，一个cachedpool可以在clear之后重复使用，以提高性能。 在cachedpool析构时，会释放所分配的所有内存

### cachedpoolappend
cachedpool的加强版，从功能上是可以替代cachedpool，支持create时设置初始长度和递增步长，以及设置一块buffer作为pool的第一块分配内存。其余功能与cachedpool相同。

### xnofreepool
这个pool托管了void *buf, 并维护一个指针,每次分配,指针都往后挪对应的大小,然后把这块内存给用户
当内存free的时候,并不回收内存, 也就是那个指针 只能 一直往后挪, 直到buffer分配光
有clear方法,调用clear之后,指针=0,然后从头开始分配.(备注:实际是从尾巴开始分配)

### xmempool
这个pool,也是托管了buffer, 而且有clear方法
这个buffer,跟xnofreepool,区别是,它可以回收并重复利用内存

### xcompool
这个pool,不是托管buffur,而是内部维护buffer
因此,它功能跟着xmempool一样,但是内部会动态扩展内存,因此不受外部buffer大小限制

### pool比较

|pool	|分配速度|释放速度|内存可回收|内存可统一清空|线程安全|备注|
|------|-----|----|-----|-----|---|-----|
|syspool|	慢|	慢|	是|	否|	是|	如果要使用,请配合tcmalloc优化|
|cachedpool|	中等|	中等|	否|	是|	否|	动态内存扩展|
|cachedpoolappend|	快|	中等|	否|	是|	否|	内存动态扩展|
|xnofreepool|	快|	快|	否|	是|	否|	指定内存用光,就没内存了|
|xmempool|	中等|	快|	是|	是|	否|	指定内存用光,就没内存了|
|xcompool|	中等|	快|	是|	是|	否|	动态内存扩展|