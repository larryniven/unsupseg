CXXFLAGS += -std=c++11 -I ../
AR = gcc-ar

.PHONY: all clean

all: libunsupseg.a

clean:
	-rm *.o
	-rm libunsupseg.a

libunsupseg.a: dtw.o
	$(AR) rcs $@ $^

