#include <BisonParser.h>
#include <grammar/parser.hpp>

using namespace ASTree;

const CProgram* CBisonParser::buildAST( const std::string& inputFileName ) {
    CProgram* root;

    FILE* inputStream = fopen( inputFileName.c_str(), "r" );
    if ( !inputStream ) {
        throw std::invalid_argument( "Cannot open file: " + inputFileName );
    }
    yyin = inputStream;
    do {
        yyparse( &root );
    } while ( !feof( yyin ) );
    fclose( inputStream );

    return root;
}
