CFLAGS=-g -std=c99 -O2 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wunreachable-code -Wno-unused-parameter -Wpedantic
override LDFLAGS+= -L.
override CFLAGS+= -fPIC -I.
TESTS=$(patsubst tests/test_helpers,,$(patsubst %.c,%,$(wildcard tests/*.c)))

.PHONY: all
all: libbostree.so libbostree.a $(TESTS)

.PHONY: test
test: $(TESTS)
	@for test in $(TESTS); do \
		echo "--- Starting: $$test"; \
		$$test; \
		if [ "$$?" != "0" ]; then \
			echo; \
			echo "FAILED"; \
			exit 1; \
		else \
			echo "--- $$test passed"; \
			echo; \
		fi; \
	done; \
	echo; \
	echo "All tests passed."

libbostree.so: bostree.o
	$(LD) -shared -o $@ $+
	ln -s ../$@ tests/ || true

libbostree.a: bostree.o
	$(AR) rcs $@ $+
	ranlib $@

tests/test_helpers.o: tests/test_helpers.c
	$(CC) -c $(CFLAGS) -o $@ $+

%: %.c tests/test_helpers.o libbostree.so
	$(CC) $(CFLAGS) -o $@ $< tests/test_helpers.o $(LDFLAGS) -Wl,-rpath=. -lbostree

.PHONY: clean

clean:
	rm -f *.o bos *.so tests/*.so tests/*.o libbostree.a $(TESTS)
