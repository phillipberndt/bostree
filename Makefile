CFLAGS=-g -Wall -fPIC -O2

.PHONY: all
all: bos libbostree.so

bos: main.o libbostree.so
	$(CC) -o $@ $< -L. -lbostree

libbostree.so: bostree.o
	$(LD) -shared -o $@ $+

.PHONY: clean

clean:
	rm -f *.o bos *.so
