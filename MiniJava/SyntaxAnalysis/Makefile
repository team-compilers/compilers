CC=g++
FLEX=flex
BISON=bison

FLEX_SRC=src/grammar/lexer.l
BISON_SRC=src/grammar/parser.y
FLEX_TRG=src/grammar/lexer.yy.cpp
BISON_TRG_CPP=src/grammar/parser.cpp
BISON_TRG_HPP=src/grammar/parser.hpp
BISON_TRG=$(BISON_TRG_HPP) $(BISON_TRG_CPP)
MAIN=src/main.cpp

TRG=ast
# -lfl includes flex library
FLAGS=-lfl -O2 -std=c++11 -I"src/astNodes" -I"src/grammar" -I"src/visitors"
SRC=$(FLEX_TRG) $(BISON_TRG_CPP) $(MAIN) src/visitors/DotLangVisitor.cpp src/visitors/Visitor.cpp src/visitors/PrintCodeVisitor.cpp src/BisonParser.cpp

$(TRG): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(TRG)

$(FLEX_TRG): $(FLEX_SRC) $(BISON_TRG_HPP)
	$(FLEX) -o $(FLEX_TRG) $(FLEX_SRC)

$(BISON_TRG): $(BISON_SRC)
	$(BISON) -d $(BISON_SRC) --verbose -o $(BISON_TRG_CPP)

clean:
	rm -f $(TRG) $(FLEX_TRG) $(BISON_TRG) src/grammar/parser.output
