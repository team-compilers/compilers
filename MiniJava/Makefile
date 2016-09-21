CC=g++
FLAGS=-O2 -std=c++11

example: lexer.yy.cpp main.cpp
	$(CC) $(FLAGS) lexer.yy.cpp main.cpp -o example

lexer.yy.cpp: lexer.l
	flex++ -o lexer.yy.cpp lexer.l

clean:
	rm -f example lexer.yy.cpp
