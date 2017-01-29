#include <fstream>
#include <memory>
#include <string>

#include <SymbolTable.h>
#include <AST/nodes/Program.h>
#include <AST/visitors/DotLangVisitor.h>
#include <AST/visitors/PrintCodeVisitor.h>
#include <AST/visitors/SymbolTableBuilderVisitor.h>
#include <AST/visitors/IrtBuilderVisitor.h>
#include <AST/visitors/TypeCheckerVisitor.h>

#include <BisonParser.h>

using namespace AstTree;

std::string AstToDotLanguage( const CProgram* root, bool verbose ) {
    CDotLangVisitor visitor( verbose );
    visitor.Visit( root );
    return visitor.GetTraversalInDotLanguage();
}

std::string AstToCode( const CProgram* root, bool verbose ) {
    CPrintCodeVisitor visitor( verbose );
    visitor.Visit( root );
    return visitor.GetCode();
}

void printResultToFile( const std::string& outputFilePath, const std::string& result ) {
    std::ofstream outStream( outputFilePath );
    outStream << result;
    outStream.close();
}

void printHelp( const std::string& programName ) {
    std::cerr << programName << " <inputFilePath> <outputFilePath> <mode>" << std::endl;
    std::cerr << "<mode>: dot / code" << std::endl;
}

template<typename T>
void extractVisitorErrorsAndPrint( T* visitor ) {
	std::shared_ptr<const std::vector<CCompilationError>> errors = visitor->Errors();
    for ( const CCompilationError& error : *errors ) {
        std::cout << error.ToString() << std::endl;
    }
}

void printErrorCount( int errorCount ) {
    std::cout << "Found "  << errorCount << " errors." << std::endl;
}

int main( int argc, char* argv[] ) {
    if ( argc < 4 ) {
        printHelp( argv[0] );
        throw std::logic_error( "Too few arguments provided" );
    }
    const std::string inputFilePath( argv[1] );
    const std::string outputFilePath( argv[2] );
    const std::string mode( argv[3] );
    CBisonParser parser( inputFilePath );
    std::unique_ptr<const CProgram> astRoot = parser.buildAST( inputFilePath );

    CSymbolTableBuilderVisitor symbolTableBuilderVisitor( false );
    symbolTableBuilderVisitor.Visit( astRoot.get() );
    std::shared_ptr<const CSymbolTable> tablePtr = symbolTableBuilderVisitor.SymbolTable();

    CIrtBuilderVisitor irtBuilderVisitor( tablePtr, true );
    irtBuilderVisitor.Visit( astRoot.get() );
    return 0;

    std::string traversal;
    if ( mode == "code" ) {
        traversal = AstToCode( astRoot.get(), false );
        printResultToFile( outputFilePath, traversal );
    } else if ( mode == "dot" ) {
        traversal = AstToDotLanguage( astRoot.get(), false );
        printResultToFile( outputFilePath, traversal );
    } else if ( mode == "errors" ) {
        std::cout << std::string( argv[1] ) << std::endl;
        CSymbolTableBuilderVisitor symbolTableBuilderVisitor( false );
        symbolTableBuilderVisitor.Visit( astRoot.get() );
        std::shared_ptr<const CSymbolTable> tablePtr = symbolTableBuilderVisitor.SymbolTable();

        int errorCount = symbolTableBuilderVisitor.Errors()->size();
        if ( errorCount == 0 ) {
            CTypeCheckerVisitor typeCheckerVisitor( tablePtr, false );
            typeCheckerVisitor.Visit( astRoot.get() );
            errorCount += typeCheckerVisitor.Errors()->size();
            printErrorCount( errorCount );
            extractVisitorErrorsAndPrint( &symbolTableBuilderVisitor );
            extractVisitorErrorsAndPrint( &typeCheckerVisitor );
        } else {
            printErrorCount( errorCount );
            extractVisitorErrorsAndPrint( &symbolTableBuilderVisitor );
        }
        std::cout << std::endl;
    } else {
        printHelp( argv[0] );
        throw std::logic_error( "Wrong mode provided" );
    }

    return 0;
}
