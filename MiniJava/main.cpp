#include <stdio.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <Program.h>
#include <PrintVisitor.h>

extern "C" int yyparse( CProgram** root );
extern "C" FILE* yyin;
extern "C" FILE* yyout;
extern int lineNum;

void yyerror( CProgram** root, const char* message ) {
    std::cout << "Parse error at line " << lineNum << ".  Message: " << message << std::endl;
}

std::unique_ptr<const CProgram> buildAST( const std::string& inputFileName, const std::string& outputFileName ) {
    CProgram* root;

    FILE* inputStream = fopen( inputFileName.c_str(), "r" );
    if ( !inputStream ) {
        throw std::invalid_argument( "Cannot open file: " + inputFileName );
    }
    yyin = inputStream;
    do {
        yyparse(&root);
    } while ( !feof( yyin ) );
    fclose( inputStream );

    return std::unique_ptr<const CProgram>(root);
}

void printAST( const CProgram* root ) {
    CPrintVisitor visitor;
    // visitor.Visit( root );
    std::cout << root << std::endl;
}

int main() {
    std::string inputFileName = "tmp/dull.java";
    std::string outputFileName = "tmp/tokenized.txt";
    std::unique_ptr<const CProgram> astRoot = buildAST( inputFileName, outputFileName );
    printAST( astRoot.get() );
    return 0;
}
