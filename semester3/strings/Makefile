all : bin/* 
bin/% : %.cpp functions.h
	mkdir -p bin/
	g++ -g -std=c++0x $^ -o $@
