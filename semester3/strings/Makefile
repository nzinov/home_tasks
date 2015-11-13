all : bin/* 
bin/% : %.cpp functions.h
	mkdir -p bin/
	g++ -g -std=c++0x $^ -o $@
ejudge/%.cpp : %.cpp functions.h
	mkdir -p ejudge/
	cat functions.h $< | sed '/functions.h/d' > $@
