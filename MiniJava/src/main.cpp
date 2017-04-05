#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <Compiler.h>

void printHelp( const std::string& programName ) {
    std::cerr << programName << " <inputFilePath> <outputDirPath>" << std::endl;
}

int main( int argc, char* argv[] ) {
    if ( argc < 3 ) {
        printHelp( argv[0] );
        throw std::logic_error( "Too few arguments provided" );
    }
    const std::string pathInputFile( argv[1] );
    const std::string pathOutputDir( argv[2] );

    CCompiler compiler( pathInputFile, pathOutputDir );
    compiler.Run();

    return 0;
}
