bin/main:
	mkdir -p bin;
	g++ main.cpp --std=c++11 -o bin/main
test: bin/main
	bin/main < test.in | diff -y - test.out
