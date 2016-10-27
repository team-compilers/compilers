CC=g++
FLAGS=-O2 -std=c++11 -I"inc"
SRC=lexer.yy.cpp parser.cpp

example: $(SRC)
	$(CC) $(FLAGS) $(SRC)

lexer.yy.cpp: lexer.l
	flex++ -o lexer.yy.cpp lexer.l

parser.cpp: parser.y
	bison -d parser.y --verbose -o parser.cpp

clean:
	rm -f example lexer.yy.cpp parser.cpp
