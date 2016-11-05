#include <stdio.h>
#include <iostream>
#include <string>
#include <stdexcept>

extern "C" int yyparse();
extern "C" FILE* yyin;
extern "C" FILE* yyout;
extern int line_num;

void yyerror(const char *s) {
    std::cout << "Parse error at line " << line_num << ".  Message: " << s << std::endl;
    // might as well halt now:
    // exit(-1);
}

void lexicalAnalysis( const std::string& inputFileName, const std::string& outputFileName ) {
    FILE* inputStream = fopen(inputFileName.c_str(), "r");
    if (!inputStream) {
        throw std::invalid_argument( "Cannot open file: " + inputFileName );
    }
    yyin = inputStream;
    do {
        yyparse();
    } while (!feof(yyin));
    // std::ifstream inputStream( inputFileName.c_str() );
    // std::ofstream outputStream( outputFileName.c_str() );
    // if ( inputStream.good() ) {
        // if ( outputStream.good() ) {
        //     yyFlexLexer lexer( inputStream, outputStream );
        //     while ( lexer.yylex() != EOF );
        //     inputStream.close();
        //     outputStream.close();
        // } else {
        //     throw std::invalid_argument( "Cannot open file: " + inputFileName );
        // }
        // yyin = inputStream;
        // yylex();
    // } else {
        // throw std::invalid_argument( "Cannot open file: " + inputFileName );
    // }
}

int main() {
    std::string inputFileName = "tmp/sample.java";
    std::string outputFileName = "tmp/tokenized.txt";
    lexicalAnalysis(inputFileName, outputFileName);
    return 0;
}
