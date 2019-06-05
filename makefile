all: calc

calc: parser.c scanner.c
	gcc -o calc scanner.c parser.c

parser.c: parser.y hashTable.c hashTable.h
	bison --yacc --defines --output=parser.c parser.y

scanner.c: scanner.l
	flex --outfile=scanner.c scanner.l

clean:
	rm calc parser.c scanner.c parser.h
