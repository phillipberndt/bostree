CFLAGS=-g -Wall -fPIC -O2
TESTS=$(patsubst %.c,%,$(wildcard tests/*.c))

.PHONY: all
all: libbostree.so $(TESTS)

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

%: %.c libbostree.so
	$(CC) -L. -I. -o $@ $< -lbostree

.PHONY: clean

clean:
	rm -f *.o bos *.so tests/*.so $(TESTS)
