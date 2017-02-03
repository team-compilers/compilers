#include <CompilationPhase.h>

#include <iostream>
#include <fstream>

#include <BisonParser.h>

void CAstBuildingPhase::Run() {
    CBisonParser parser( pathInputFile );
    astRoot = std::shared_ptr<const ASTree::CProgram>( parser.buildAST( pathInputFile ) );
}

void CAstBuildingPhase::PrintResults( const std::string& pathOutputFile, const std::ios_base::openmode& openMode ) {
    std::fstream outputStream( pathOutputFile, openMode );

    outputStream << ToDotLanguage() << std::endl;

    outputStream.close();
}

std::shared_ptr<const ASTree::CProgram> CAstBuildingPhase::GetAstRoot() const {
    return astRoot;
}

std::string CAstBuildingPhase::ToDotLanguage() {
    assert( astRoot != nullptr );
    if ( dotLangTraversal.empty() ) {
        astRoot->Accept( &dotLangVisitor );
        dotLangTraversal = dotLangVisitor.GetTraversalInDotLanguage();
    }
    return dotLangTraversal;
}

void CSymbolTableBuildingPhase::Run() {
    symbolTableBuilderVisitor.Visit( astRoot.get() );
}

void CSymbolTableBuildingPhase::PrintResults( const std::string& pathOutputFile, const std::ios_base::openmode& openMode ) {
    std::shared_ptr<const std::vector<CCompilationError>> errors = GetErrors();
    if ( !errors->empty() ) {
        std::fstream outputStream( pathOutputFile, openMode );
        for ( auto it = errors->begin(); it != errors->end(); ++it ) {
            outputStream << it->ToString() << std::endl;
        }
        outputStream.close();
    }
}

std::shared_ptr<const CSymbolTable> CSymbolTableBuildingPhase::GetSymbolTable() const {
    return symbolTableBuilderVisitor.SymbolTable();
}

std::shared_ptr<const std::vector<CCompilationError>> CSymbolTableBuildingPhase::GetErrors() const {
    return symbolTableBuilderVisitor.Errors();
}

void CTypeCheckingPhase::Run() {
    typeCheckerVisitor.Visit( astRoot.get() );
}

void CTypeCheckingPhase::PrintResults( const std::string& pathOutputFile, const std::ios_base::openmode& openMode ) {
    std::shared_ptr<const std::vector<CCompilationError>> errors = GetErrors();
    if ( !errors->empty() ) {
        std::fstream outputStream( pathOutputFile, openMode );
        for ( auto it = errors->begin(); it != errors->end(); ++it ) {
            outputStream << it->ToString() << std::endl;
        }
        outputStream.close();
    }
}

std::shared_ptr<const std::vector<CCompilationError>> CTypeCheckingPhase::GetErrors() const {
    return typeCheckerVisitor.Errors();
}

void CIrtBuildingPhase::Run() {
    irtBuilderVisitor.Visit( astRoot.get() );
    methodTrees = irtBuilderVisitor.MethodTrees();
}

void CIrtBuildingPhase::PrintResults( const std::string& pathOutputFile, const std::ios_base::openmode& openMode ) {
    std::fstream outputStream( pathOutputFile, openMode );

    outputStream.close();
}

std::shared_ptr<const std::unordered_map<std::string, std::shared_ptr<const IRTree::CStatement>>> CIrtBuildingPhase::MethodTrees() const {
    return methodTrees;
}

std::string CIrtBuildingPhase::ToDotLanguage( const std::string& methodName ) {
    if ( dotLangTraversal.empty() ) {
        methodTrees->at( methodName )->Accept( &dotLangVisitor );
        dotLangTraversal = dotLangVisitor.GetTraversalInDotLanguage();
    }
    return dotLangTraversal;
}
