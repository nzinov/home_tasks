all : bin/*
bin/main : main.cpp matcher.h matcher.cpp aho.h aho.cpp
	mkdir -p bin/
	g++ -o bin/main -std=c++11 -g main.cpp matcher.cpp aho.cpp
bin/ejudge : ejudge.cpp
	mkdir -p bin/
	g++ -o bin/ejudge -std=c++11 -g ejudge.cpp
bin/test : test.cpp matcher.h matcher.cpp aho.h aho.cpp
	mkdir -p bin/
	g++ -o bin/test -std=c++11 -g test.cpp matcher.cpp aho.cpp
bin/test2 : test2.cpp matcher.h matcher.cpp aho.h aho.cpp
	mkdir -p bin/
	g++ -o bin/test2 -std=c++11 -g test2.cpp matcher.cpp aho.cpp
