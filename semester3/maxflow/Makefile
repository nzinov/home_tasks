bin/ejudge: ejudge.cpp
	mkdir -p bin;
	g++ ejudge.cpp -g --std=c++0x -o bin/ejudge
bin/main: main.cpp graph.h
	mkdir -p bin;
	g++ main.cpp -g --std=c++0x -o bin/main
test: bin/ejudge
	bin/ejudge < test.in | diff -y - test.out
debug: bin/main
	gdb bin/ejudge
