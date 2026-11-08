CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -O2
CPP=-Iinclude
COMMON=src/http.c src/multipart.c src/flow.c src/response.c
.PHONY: all test clean
all: build/server build/client build/tests build/benchmark
build:
	mkdir -p build
build/server: $(COMMON) src/server.c include/lab.h | build
	$(CC) $(CFLAGS) $(CPP) $(COMMON) src/server.c -o $@
build/client: src/client.c | build
	$(CC) $(CFLAGS) src/client.c -o $@
build/tests: $(COMMON) tests/test_lab.c include/lab.h | build
	$(CC) $(CFLAGS) $(CPP) $(COMMON) tests/test_lab.c -o $@
build/benchmark: $(COMMON) src/benchmark.c include/lab.h | build
	$(CC) $(CFLAGS) $(CPP) $(COMMON) src/benchmark.c -o $@
test: build/tests
	./build/tests
clean:
	rm -rf build
