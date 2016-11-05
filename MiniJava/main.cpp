#include <stdio.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <Program.h>
// #include <PrintVisitor.h>

extern "C" int yyparse();
extern "C" FILE* yyin;
extern "C" FILE* yyout;
extern int lineNum;

std::unique_ptr<const CProgram> astRoot;

void yyerror( const char* message ) {
    std::cout << "Parse error at line " << lineNum << ".  Message: " << message << std::endl;
}

void lexicalAnalysis( const std::string& inputFileName, const std::string& outputFileName ) {
    FILE* inputStream = fopen( inputFileName.c_str(), "r" );
    if ( !inputStream ) {
        throw std::invalid_argument( "Cannot open file: " + inputFileName );
    }
    yyin = inputStream;
    do {
        yyparse();
    } while ( !feof( yyin ) );
    fclose( inputStream );
}

void buildAST( const CProgram* root ) {
    // CPrintVisitor visitor;
    // visitor.Visit( root );
    std::cout << root << std::endl;
}

int main() {
    std::string inputFileName = "tmp/dull.java";
    std::string outputFileName = "tmp/tokenized.txt";
    lexicalAnalysis( inputFileName, outputFileName );
    buildAST( astRoot.get() );
    return 0;
}
