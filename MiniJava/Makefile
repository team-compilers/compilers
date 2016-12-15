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

TRG=minijava
# -lfl includes flex library
FLAGS=-lfl -O2 -std=c++11 -I"src/" -I"src/grammar"
SRC=$(FLEX_TRG) $(BISON_TRG_CPP) $(MAIN) src/AST/nodes/NodeNames.cpp src/AST/visitors/Visitor.cpp src/AST/visitors/DotLangVisitor.cpp src/AST/visitors/TypeCheckerVisitor.cpp src/AST/visitors/PrintCodeVisitor.cpp src/AST/visitors/SymbolTableBuilderVisitor.cpp src/BisonParser.cpp src/CompilationError.cpp src/SymbolTable.cpp src/IRT/nodes/NodeNames.cpp src/IRT/visitors/Visitor.cpp

$(TRG): $(SRC)
	$(CC) $(SRC) -o $(TRG) $(FLAGS)

$(FLEX_TRG): $(FLEX_SRC) $(BISON_TRG_HPP)
	$(FLEX) -o $(FLEX_TRG) $(FLEX_SRC)

$(BISON_TRG): $(BISON_SRC)
	$(BISON) -d $(BISON_SRC) --verbose -o $(BISON_TRG_CPP)

clean:
	rm -f $(TRG) $(FLEX_TRG) $(BISON_TRG) src/grammar/parser.output
