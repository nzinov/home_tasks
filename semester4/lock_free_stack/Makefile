CXXFLAGS:=$(CXXFLAGS) -std=c++17 -g
LDFLAGS:=$(LDFLAGS) -pthread -lgtest -lgtest_main

run_test : bin/test
	bin/test

bin/test : test.o
	mkdir -p bin/
	g++ $? $(CXXFLAGS) $(LDFLAGS) -o $@

test.o : lock_free_stack.h

