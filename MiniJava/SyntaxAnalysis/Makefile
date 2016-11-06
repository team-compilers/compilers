CC=g++
FLEX=flex
BISON=bison

FLEX_SRC=grammar/lexer.l
BISON_SRC=grammar/parser.y
FLEX_TRG=src/lexer.yy.cpp
BISON_TRG_CPP=src/parser.cpp
BISON_TRG_HPP=src/parser.hpp
BISON_TRG=$(BISON_TRG_HPP) $(BISON_TRG_CPP)
MAIN=src/main.cpp

TRG=ast
# -lfl includes flex library
FLAGS=-lfl -O2 -std=c++11 -I"inc"
SRC=$(FLEX_TRG) $(BISON_TRG_CPP) $(MAIN) src/PrintVisitor.cpp

$(TRG): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(TRG)

$(FLEX_TRG): $(FLEX_SRC) $(BISON_TRG_HPP)
	$(FLEX) -o $(FLEX_TRG) $(FLEX_SRC)

$(BISON_TRG): $(BISON_SRC)
	$(BISON) -d $(BISON_SRC) --verbose -o $(BISON_TRG_CPP)

clean:
	rm -f $(TRG) $(FLEX_TRG) $(BISON_TRG) src/parser.output
