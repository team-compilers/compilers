CC=g++
FLAGS=-O2 -std=c++11
SRC=TokenPrinter.cpp lexer.yy.cpp main.cpp

example: $(SRC)
	$(CC) $(FLAGS) $(SRC) -o example

lexer.yy.cpp: lexer.l
	flex++ -o lexer.yy.cpp lexer.l

clean:
	rm -f example lexer.yy.cpp
