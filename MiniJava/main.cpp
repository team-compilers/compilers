#include <dirent.h>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <FlexLexer.h>

std::vector<std::string> listDirectory( const std::string& dirName ) {
    std::vector<std::string> fileNames;

    struct dirent* dirpent;
    DIR* dirp = opendir( dirName.c_str() );
    if ( dirp ) {
        while ( ( dirpent = readdir(dirp) ) != NULL ) {
            if ( dirpent->d_type == 8 ) {
                // it's a file, not folder
                fileNames.emplace_back( dirpent->d_name );
            }
        }
        closedir( dirp );
    } else {
        throw std::invalid_argument( "Directory doesn't exist: " + dirName );
    }

    return fileNames;
}

void lexicalAnalysis( const std::string& inputFileName, const std::string& outputFileName ) {
    std::ifstream inputStream( inputFileName.c_str() );
    std::ofstream outputStream( outputFileName.c_str() );
    if ( inputStream.good() ) {
        if ( outputStream.good() ) {
            yyFlexLexer lexer( inputStream, outputStream );
            while ( lexer.yylex() != EOF );
            inputStream.close();
            outputStream.close();
        } else {
            throw std::invalid_argument( "Cannot open file: " + inputFileName );
        }
    } else {
        throw std::invalid_argument( "Cannot open file: " + inputFileName );
    }
}

int main() {
    const std::string inputDirName = "Samples";
    const std::string outputDirName = "Tokenized";
    std::vector<std::string> inputFileNames = listDirectory( inputDirName );
    for ( const std::string& inputFileName : inputFileNames ) {
        std::string outputFileName = inputFileName.substr( 0, inputFileName.find_last_of( '.' ) ) + ".txt";
        lexicalAnalysis( inputDirName + '/' + inputFileName, outputDirName + '/' + outputFileName );
    }
    return 0;
}
