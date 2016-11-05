#include <stdio.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <Program.h>

extern "C" int yyparse();
extern "C" FILE* yyin;
extern "C" FILE* yyout;
extern int line_num;

std::unique_ptr<CProgram> root;

void yyerror( const char *s ) {
    std::cout << "Parse error at line " << line_num << ".  Message: " << s << std::endl;
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

int main() {
    std::string inputFileName = "tmp/dull.java";
    std::string outputFileName = "tmp/tokenized.txt";
    lexicalAnalysis( inputFileName, outputFileName );
    std::cout << root.get() << std::endl;
    return 0;
}
