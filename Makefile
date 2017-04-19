CC=g++

INCOUT=../output/include/
LIBOUT=../output/lib/

INCLUDES= 
CFLAGS = -fsigned-char -Wall -W -pipe -Wno-unused-parameter -g -fPIC
LDFLAGS= 
OBJS=cachedpool.o xmempool.o xcompool.o cachedpoolappend.o
TARGET=
LIB= libpool.a
all : $(TARGET) $(LIB)
	mkdir -p $(INCOUT)
	mkdir -p $(LIBOUT)
	cp *.h $(INCOUT)
	cp $(LIB) $(LIBOUT)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(INCLUDES)

$(LIB) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)

tags : 
	ctags -R *

clean:
	rm -f $(OBJS) $(TARGET) $(LIB)
	rm -rf $(INCOUT)
	rm -rf $(LIBOUT)/$(LIB)

cachedpool.o: cachedpool.cpp cachedpool.h mempool.h
xcompool.o: xcompool.cpp xcompool.h xmempool.h mempool.h
xmempool.o: xmempool.cpp xmempool.h mempool.h
cachedpoolappend.o : cachedpoolappend.cpp cachedpoolappend.h mempool.h
