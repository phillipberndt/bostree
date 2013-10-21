CFLAGS=-g -Wall -fPIC -O2 -I.
TESTS=$(patsubst tests/test_helpers,,$(patsubst %.c,%,$(wildcard tests/*.c)))

.PHONY: all
all: libbostree.so libbostree.a $(TESTS)

.PHONY: test
test: $(TESTS)
	for test in $(TESTS); do \
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
	$(CC) -L. -I. -o $@ $< tests/test_helpers.o -lbostree

.PHONY: clean

clean:
	rm -f *.o bos *.so tests/*.so tests/*.o libbostree.a $(TESTS)
