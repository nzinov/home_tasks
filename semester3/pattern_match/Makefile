CXXFLAGS = -std=c++11 -g
all : bin/*
bin/main : main.o matcher.o aho.o
	$(CXX) $(CXXFLAGS) -o $@ %^
bin/ejudge : ejudge.o
	$(CXX) $(CXXFLAGS) -o $@ %^
bin/test : test.o matcher.o aho.o
	$(CXX) $(CXXFLAGS) -o $@ %^
bin/test2 : test2.o matcher.o aho.o
	$(CXX) $(CXXFLAGS) -o $@ %^
main.o : main.cpp matcher.h
ejudge.o : ejudge.cpp
matcher.o : matcher.h matcher.cpp aho.h
aho.o : aho.h aho.cpp
