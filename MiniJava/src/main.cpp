#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <Compilator.h>

// std::string AstToCode( const CProgram* root, bool verbose ) {
//     CPrintCodeVisitor visitor( verbose );
//     visitor.Visit( root );
//     return visitor.GetCode();
// }

// void printResultToFile( const std::string& outputFilePath, const std::string& result ) {
//     std::ofstream outStream( outputFilePath );
//     outStream << result;
//     outStream.close();
// }

void printHelp( const std::string& programName ) {
    std::cerr << programName << " <inputFilePath> <outputDirPath>" << std::endl;
}

// template<typename T>
// void extractVisitorErrorsAndPrint( T* visitor ) {
//     std::shared_ptr<const std::vector<CCompilationError>> errors = visitor->Errors();
//     for ( const CCompilationError& error : *errors ) {
//         std::cout << error.ToString() << std::endl;
//     }
// }

// void printErrorCount( int errorCount ) {
//     std::cout << "Found "  << errorCount << " errors." << std::endl;
// }

int main( int argc, char* argv[] ) {
    if ( argc < 3 ) {
        printHelp( argv[0] );
        throw std::logic_error( "Too few arguments provided" );
    }
    const std::string pathInputFile( argv[1] );
    const std::string pathOutputDir( argv[2] );

    CCompilator compilator( pathInputFile, pathOutputDir );
    // CIrtBuilderVisitor irtBuilderVisitor( tablePtr, false );
    // irtBuilderVisitor.Visit( astRoot.get() );
    // auto irtTrees = irtBuilderVisitor.MethodTrees();
    // std::shared_ptr<const IRTree::CStatement> irtRootSomeMethod = irtTrees.begin()->second;

    // IRTree::CDotLangVisitor irtVisitor( false );
    // irtRootSomeMethod->Accept( &irtVisitor );
    // std::string traversal = irtVisitor.GetTraversalInDotLanguage();
    // printResultToFile( outputFilePath, traversal );
    // return 0;

    // std::string traversal;
    // if ( mode == "code" ) {
    //     traversal = AstToCode( astRoot.get(), false );
    //     printResultToFile( outputFilePath, traversal );
    // } else if ( mode == "dot" ) {
    //     traversal = AstToDotLanguage( astRoot.get(), false );
    //     printResultToFile( outputFilePath, traversal );
    // } else if ( mode == "errors" ) {
    //     std::cout << std::string( argv[1] ) << std::endl;
    //     CSymbolTableBuilderVisitor symbolTableBuilderVisitor( false );
    //     symbolTableBuilderVisitor.Visit( astRoot.get() );
    //     std::shared_ptr<const CSymbolTable> tablePtr = symbolTableBuilderVisitor.SymbolTable();

    //     int errorCount = symbolTableBuilderVisitor.Errors()->size();
    //     if ( errorCount == 0 ) {
    //         CTypeCheckerVisitor typeCheckerVisitor( tablePtr, false );
    //         typeCheckerVisitor.Visit( astRoot.get() );
    //         errorCount += typeCheckerVisitor.Errors()->size();
    //         printErrorCount( errorCount );
    //         extractVisitorErrorsAndPrint( &symbolTableBuilderVisitor );
    //         extractVisitorErrorsAndPrint( &typeCheckerVisitor );
    //     } else {
    //         printErrorCount( errorCount );
    //         extractVisitorErrorsAndPrint( &symbolTableBuilderVisitor );
    //     }
    //     std::cout << std::endl;
    // } else {
    //     printHelp( argv[0] );
    //     throw std::logic_error( "Wrong mode provided" );
    // }

    return 0;
}
