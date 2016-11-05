CC=g++
FLEX=flex
BISON=bison
# -lfl includes flex library
FLAGS=-lfl -O2 -std=c++11 -I"inc"
SRC=lexer.yy.cpp parser.cpp main.cpp

prog: $(SRC)
	$(CC) $(FLAGS) $(SRC) -o prog

lexer.yy.cpp: lexer.l parser.hpp
	$(FLEX) -o lexer.yy.cpp lexer.l

parser.cpp parser.hpp: parser.y
	$(BISON) -d parser.y --verbose -o parser.cpp

clean:
	rm -f prog lexer.yy.cpp parser.cpp parser.hpp
