CFLAGS=-g -Wall -fPIC

.PHONY: all
all: bos bostree.so

bos: main.o bostree.so
	$(CC) -o $@ $+

bostree.so: bostree.o
	$(LD) -shared -o $@ $+

.PHONY: clean

clean:
	rm -f *.o bos *.so
