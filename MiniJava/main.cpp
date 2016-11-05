#include <stdio.h>
#include <iostream>
#include <string>
#include <stdexcept>

extern "C" int yyparse();
extern "C" FILE* yyin;
extern "C" FILE* yyout;
extern int line_num;

void yyerror( const char *s ) {
    std::cout << "Parse error at line " << line_num << ".  Message: " << s << std::endl;
    // might as well halt now:
    // exit(-1);
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
    return 0;
}
