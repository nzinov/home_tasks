bin/main: main.cpp graph.h
	mkdir -p bin;
	g++ main.cpp -g --std=c++11 -o bin/main
test: bin/main
	bin/main < test.in | diff -y - test.out
debug: bin/main
	gdb bin/main
