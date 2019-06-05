all: calc 

calc: bison flex
	g++ -g -std=c++14 -o calc parser.cpp scanner.cpp

bison: parser.y
	bison --yacc --defines -o parser.cpp parser.y

flex: scanner.l
	flex -o scanner.cpp scanner.l

clean:
	rm calc parser.cpp scanner.cpp parser.hpp
