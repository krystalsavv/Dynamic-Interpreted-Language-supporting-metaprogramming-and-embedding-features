all: calc

calc: parser.cpp scanner.cpp
	g++ -o calc scanner.cpp parser.cpp

parser.c: parser.y hashTable.c hashTable.h
	bison++ --yacc --defines --output=parser.cpp parser.y

scanner.c: scanner.l
	flex++ --outfile=scanner.cpp scanner.l

clean:
	rm calc parser.c scanner.c parser.h
