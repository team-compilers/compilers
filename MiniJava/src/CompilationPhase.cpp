#include <CompilationPhase.h>

#include <iostream>
#include <fstream>

#include <AST/visitors/DotLangVisitor.h>
#include <AST/visitors/GenerateCodeVisitor.h>
#include <AST/visitors/SymbolTableBuilderVisitor.h>
#include <AST/visitors/IrtBuilderVisitor.h>
#include <AST/visitors/TypeCheckerVisitor.h>

#include <IRT/visitors/DotLangVisitor.h>
#include <IRT/visitors/DoubleCallEliminationVisitor.h>
#include <IRT/visitors/EseqEliminationVisitor.h>

#include <BisonParser.h>

void CAstBuildingPhase::Run() {
    CBisonParser parser( pathInputFile );
    astRoot = std::unique_ptr<const ASTree::CProgram>( parser.buildAST( pathInputFile ) );
}

void CAstBuildingPhase::PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    std::fstream outputStream( pathOutputFile + extension, openMode );
    outputStream << ToDotLanguage() << std::endl;
    outputStream.close();
}

void CAstBuildingPhase::PrintCodeGenerated( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    std::fstream outputStream( pathOutputFile + extension, openMode );
    outputStream << ToCode() << std::endl;
    outputStream.close();
}

const ASTree::CProgram* CAstBuildingPhase::GetAstRoot() const {
    return astRoot.get();
}

std::string CAstBuildingPhase::ToDotLanguage() {
    assert( astRoot );
    if ( dotLangTraversal.empty() ) {
        ASTree::CDotLangVisitor dotLangVisitor( verbose );
        astRoot->Accept( &dotLangVisitor );
        dotLangTraversal = dotLangVisitor.GetTraversalInDotLanguage();
    }
    return dotLangTraversal;
}

std::string CAstBuildingPhase::ToCode() {
    assert( astRoot );
    if ( codeGenerated.empty() ) {
        ASTree::CGenerateCodeVisitor generateCodeVisitor( verbose );
        astRoot->Accept( &generateCodeVisitor );
        codeGenerated = generateCodeVisitor.GetCode();
    }
    return codeGenerated;
}

void CSymbolTableBuildingPhase::Run() {
    ASTree::CSymbolTableBuilderVisitor symbolTableBuilderVisitor( verbose );
    symbolTableBuilderVisitor.Visit( astRoot );
    symbolTable = std::unique_ptr<const CSymbolTable>( symbolTableBuilderVisitor.SymbolTable() );
    errors = std::unique_ptr<const std::vector<CCompilationError>>( symbolTableBuilderVisitor.Errors() );
}

void CSymbolTableBuildingPhase::PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    assert( errors );
    if ( !errors->empty() ) {
        std::fstream outputStream( pathOutputFile + extension, openMode );
        for ( auto it = errors->begin(); it != errors->end(); ++it ) {
            outputStream << it->ToString() << std::endl;
        }
        outputStream.close();
    }
}

const CSymbolTable* CSymbolTableBuildingPhase::GetSymbolTable() const {
    assert( symbolTable );
    return symbolTable.get();
}

const std::vector<CCompilationError>* CSymbolTableBuildingPhase::GetErrors() const {
    assert( errors );
    return errors.get();
}

void CTypeCheckingPhase::Run() {
    ASTree::CTypeCheckerVisitor typeCheckerVisitor( symbolTable, verbose );
    typeCheckerVisitor.Visit( astRoot );
    errors = std::unique_ptr<const std::vector<CCompilationError>>( typeCheckerVisitor.GetErrors() );
}

void CTypeCheckingPhase::PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    assert( errors );
    if ( !errors->empty() ) {
        std::fstream outputStream( pathOutputFile + extension, openMode );
        for ( auto it = errors->begin(); it != errors->end(); ++it ) {
            outputStream << it->ToString() << std::endl;
        }
        outputStream.close();
    }
}

const std::vector<CCompilationError>* CTypeCheckingPhase::GetErrors() const {
    assert( errors );
    return errors.get();
}

using TMethodToIRTMap = std::unordered_map<std::string, std::unique_ptr<const IRTree::CStatement>>;

void CIrtBuildingPhase::Run() {
    ASTree::CIrtBuilderVisitor irtBuilderVisitor( symbolTable, verbose );
    irtBuilderVisitor.Visit( astRoot );
    methodTrees = std::move( irtBuilderVisitor.MethodTrees() );
}

void CIrtBuildingPhase::PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    const TMethodToIRTMap* methodTreesPtr = MethodTrees();
    for ( auto it = methodTreesPtr->begin(); it != methodTreesPtr->end(); ++it ) {
        std::string methodName = it->first;
        methodName[0] = std::toupper( methodName[0] );
        std::fstream outputStream( pathOutputFile + methodName + extension, openMode );
        outputStream << ToDotLanguage( it->first ) << std::endl;
        outputStream.close();
    }
}

const TMethodToIRTMap* CIrtBuildingPhase::MethodTrees() const {
    assert( methodTrees );
    return methodTrees.get();
}

std::string CIrtBuildingPhase::ToDotLanguage( const std::string& methodName ) {
    assert( methodTrees );
    IRTree::CDotLangVisitor dotLangVisitor( verbose );
    methodTrees->at( methodName )->Accept( &dotLangVisitor );
    return dotLangVisitor.GetTraversalInDotLanguage();
}

void CIrtCanonizationPhase::Run() {
    for ( auto it = methodTrees->begin(); it != methodTrees->end(); ++it ) {
        IRTree::CDoubleCallEliminationVisitor callEliminationVisitor( verbose );
        it->second->Accept( &callEliminationVisitor );
        methodTreesWithoutDoubleCalls->emplace( it->first, std::move( callEliminationVisitor.ResultTree() ) );

        IRTree::CEseqEliminationVisitor eseqEliminationVisitor( verbose );
        it->second->Accept( &eseqEliminationVisitor );
    }
}

void CIrtCanonizationPhase::PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    assert( methodTreesWithoutDoubleCalls );
    for ( auto it = methodTreesWithoutDoubleCalls->begin(); it != methodTreesWithoutDoubleCalls->end(); ++it ) {
        std::string methodName = it->first;
        methodName[0] = std::toupper( methodName[0] );
        std::fstream outputStream( pathOutputFile + methodName + extension, openMode );
        outputStream << ToDotLanguage( it->first ) << std::endl;
        outputStream.close();
    }
}

std::string CIrtCanonizationPhase::ToDotLanguage( const std::string& methodName ) {
    assert( methodTreesWithoutDoubleCalls );
    IRTree::CDotLangVisitor dotLangVisitor( verbose );
    methodTreesWithoutDoubleCalls->at( methodName )->Accept( &dotLangVisitor );
    return dotLangVisitor.GetTraversalInDotLanguage();
}
