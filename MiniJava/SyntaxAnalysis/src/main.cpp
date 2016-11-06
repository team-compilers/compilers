#include <dirent.h>
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

std::vector<std::string> listDirectory( const std::string& dirName ) {
    std::vector<std::string> fileNames;

    struct dirent* dirpent;
    DIR* dirp = opendir( dirName.c_str() );
    if ( dirp ) {
        while ( ( dirpent = readdir( dirp ) ) != NULL ) {
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

std::unique_ptr<const CProgram> buildAST( const std::string& inputFileName ) {
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

    return std::unique_ptr<const CProgram>( root );
}

std::string traverseAST( const CProgram* root, bool verbose ) {
    CPrintVisitor visitor( verbose );
    visitor.Visit( root );
    return visitor.GetTraversalInDotLanguage();
}

int main() {
    const std::string inputDirName = "data/Samples";
    const std::string outputDirName = "data/SamplesGV";
    std::vector<std::string> inputFileNames = listDirectory( inputDirName );
    for ( const std::string& inputFileName : inputFileNames ) {
        std::string inputPath = inputDirName + '/' + inputFileName;
        std::string outputFileName = inputFileName.substr( 0, inputFileName.find_last_of( '.' ) ) + ".gv";
        std::string outputPath = outputDirName + '/' + outputFileName;

        std::cout << "\e[1;34m" << inputPath << "\e[1;39m" << std::endl;
        std::unique_ptr<const CProgram> astRoot = buildAST( inputPath );
        std::string traversal = traverseAST( astRoot.get(), false );

        std::ofstream outStream( outputPath );
        outStream << traversal;
        outStream.close();
    }

    return 0;
}
