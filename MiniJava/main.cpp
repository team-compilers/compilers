#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>

#include <Program.h>
#include <PrintVisitor.h>

extern "C" int yyparse( CProgram** root );
extern "C" FILE* yyin;
extern "C" FILE* yyout;
extern int lineNum;

void yyerror( CProgram** root, const char* message ) {
    std::cout << "Parse error at line " << lineNum << ".  Message: " << message << std::endl;
}

std::unique_ptr<const CProgram> buildAST( const std::string& inputFileName ) {
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

std::string traverseAST( const CProgram* root, bool verbose ) {
    CPrintVisitor visitor( verbose );
    visitor.Visit( root );
    return visitor.GetTraversalInDotLanguage();
}

int main() {
    std::string inputFileName = "tmp/dull.java";
    std::string outputFileName = "tmp/dull.gv";
    std::unique_ptr<const CProgram> astRoot = buildAST( inputFileName );
    std::string traversal = traverseAST( astRoot.get(), true );

    std::ofstream outStream( outputFileName );
    outStream << traversal;
    outStream.close();

    return 0;
}
