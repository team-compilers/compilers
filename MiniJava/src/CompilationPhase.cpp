#include <CompilationPhase.h>

#include <BisonParser.h>

void CAstBuildingPhase::Run() {
    CBisonParser parser( pathInputFile );
    astRoot = std::shared_ptr<const ASTree::CProgram>( parser.buildAST( pathInputFile ) );
}

std::shared_ptr<const ASTree::CProgram> CAstBuildingPhase::GetAstRoot() const {
    return astRoot;
}

std::string CAstBuildingPhase::ToDotLanguage() {
    if ( dotLangTraversal.empty() ) {
        astRoot->Accept( &dotLangVisitor );
        dotLangTraversal = dotLangVisitor.GetTraversalInDotLanguage();
    }
    return dotLangTraversal;
}

void CSymbolTableBuildingPhase::Run() {
    symbolTableBuilderVisitor.Visit( astRoot.get() );
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

std::shared_ptr<const std::vector<CCompilationError>> CTypeCheckingPhase::GetErrors() const {
    return typeCheckerVisitor.Errors();
}

void CIrtBuildingPhase::Run() {
    irtBuilderVisitor.Visit( astRoot.get() );
    methodTrees = irtBuilderVisitor.MethodTrees();
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
