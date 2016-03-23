CXXFLAGS:=$(CXXFLAGS) -std=c++17
LDFLAGS:=$(LDFLAGS) -pthread -lgtest -lgtest_main
object = main.o test.o
run_test : bin/test
	bin/test

bin/test : test.o
	mkdir -p bin
	g++ test.o $(LDFLAGS) -o $@
$(object) : sync_container.h
