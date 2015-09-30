bin/main: main.cpp graph.h
	mkdir -p bin;
	g++ main.cpp -g --std=c++0x -o bin/main
test: bin/main
	bin/main < test.in | diff -y - test.out
debug: bin/main
	gdb bin/main
