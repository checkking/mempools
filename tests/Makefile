
INCLUDE_PATH = -I../output/include/ \
			   -I./
LIB_PATH = -L../output/lib -lpool \
		   -lpthread \
		   -lrt
CPP = g++
CPPFLAGS = -Wall -g $(INCLUDE_PATH) \
		   -W \
		   -Wno-unused

ALLTESTS=$(wildcard *_test.cpp)
CPPFILE=$(ALLTESTS)
EXE=$(basename $(ALLTESTS))

all : $(EXE)
$(EXE) : %: %.cpp $(OBJ)
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIB_PATH)

.PHONY : clean all test

clean:
	rm -rf $(EXE) core.* log *.o *.log
