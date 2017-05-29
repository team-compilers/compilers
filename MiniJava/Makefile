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
# -g -O0 used for debug
FLAGS=-std=c++11 -I"src/" -I"src/grammar" -g -O0
SRC=$(FLEX_TRG) $(BISON_TRG_CPP) $(MAIN) \
		src/Compiler.cpp src/CompilationPhase.cpp src/FileUtils.cpp \
		src/AST/nodes/NodeNames.cpp src/AST/visitors/Visitor.cpp src/AST/visitors/DotLangVisitor.cpp \
		src/AST/visitors/TypeCheckerVisitor.cpp src/AST/visitors/GenerateCodeVisitor.cpp \
		src/AST/visitors/SymbolTableBuilderVisitor.cpp src/AST/visitors/IrtBuilderVisitor.cpp \
		src/BisonParser.cpp src/CompilationError.cpp src/SymbolTable.cpp \
		src/IRT/nodes/NodeNames.cpp src/IRT/nodes/Expression.cpp src/IRT/nodes/Statement.cpp \
		src/IRT/nodes/ExpressionList.cpp src/IRT/SubtreeWrapper.cpp src/IRT/Label.cpp \
		src/IRT/Temporary.cpp src/IRT/Frame.cpp src/IRT/visitors/Visitor.cpp \
		src/IRT/visitors/DotLangVisitor.cpp src/IRT/visitors/DoubleCallEliminationVisitor.cpp \
		src/IRT/visitors/EseqEliminationVisitor.cpp src/IRT/visitors/SeqLinearizerVisitor.cpp \
		src/IRT/visitors/TraceFormationVisitor.cpp src/IRT/visitors/BlockRearrangeVisitor.cpp \
		src/Synthesis/nodes/Commands.cpp src/Synthesis/nodes/Patterns.cpp src/Synthesis/Trace.cpp \
		src/Synthesis/visitors/Visitor.cpp src/Synthesis/visitors/TilingVisitor.cpp \
		src/Synthesis/visitors/CommandEmitterVisitor.cpp src/Synthesis/visitors/DotLangVisitor.cpp \
		src/Synthesis/nodes/NodeNames.cpp

$(TRG): $(SRC)
	$(CC) $(SRC) -o $(TRG) $(FLAGS)

$(FLEX_TRG): $(FLEX_SRC) $(BISON_TRG_HPP)
	$(FLEX) -o $(FLEX_TRG) $(FLEX_SRC)

$(BISON_TRG): $(BISON_SRC)
	$(BISON) -d $(BISON_SRC) --verbose -o $(BISON_TRG_CPP)

clean:
	rm -f $(TRG) $(FLEX_TRG) $(BISON_TRG) src/grammar/parser.output
