#pragma once

#include <memory>
#include <string>

#include <AST/nodes/Program.h>
#include <AST/visitors/DotLangVisitor.h>
#include <AST/visitors/PrintCodeVisitor.h>
#include <AST/visitors/SymbolTableBuilderVisitor.h>
#include <AST/visitors/IrtBuilderVisitor.h>
#include <AST/visitors/TypeCheckerVisitor.h>

#include <IRT/visitors/DotLangVisitor.h>
#include <SymbolTable.h>

class CCompilationPhase {
public:
    CCompilationPhase( bool _verbose = false ) : verbose( _verbose ) {}
    virtual ~CCompilationPhase() = default;

    virtual void Run() = 0;
protected:
    bool verbose;
};

class CAstBuildingPhase : public CCompilationPhase {
public:
    CAstBuildingPhase( const std::string& _pathInputFile, bool _verbose = false )
        : CCompilationPhase( _verbose ), pathInputFile( _pathInputFile ) {}

    virtual void Run() override;

    std::shared_ptr<const ASTree::CProgram> GetAstRoot() const;
    std::string ToDotLanguage();
private:
    std::string pathInputFile;
    std::shared_ptr<const ASTree::CProgram> astRoot;

    ASTree::CDotLangVisitor dotLangVisitor;
    std::string dotLangTraversal;
};

class CSymbolTableBuildingPhase : public CCompilationPhase {
public:
    CSymbolTableBuildingPhase( std::shared_ptr<const ASTree::CProgram> _astRoot, bool _verbose = false )
        : CCompilationPhase( _verbose ), symbolTableBuilderVisitor( _verbose ), astRoot( _astRoot ) {}

    virtual void Run() override;

    std::shared_ptr<const CSymbolTable> GetSymbolTable() const;
    std::shared_ptr<const std::vector<CCompilationError>> GetErrors() const;
private:
    std::shared_ptr<const ASTree::CProgram> astRoot;
    ASTree::CSymbolTableBuilderVisitor symbolTableBuilderVisitor;
};

class CTypeCheckingPhase : public CCompilationPhase {
public:
    CTypeCheckingPhase( std::shared_ptr<const ASTree::CProgram> _astRoot,
            std::shared_ptr<const CSymbolTable> _symbolTable,
            bool _verbose = false )
        : CCompilationPhase( _verbose ), typeCheckerVisitor( _symbolTable, _verbose ), astRoot( _astRoot ) {}

    virtual void Run() override;

    std::shared_ptr<const std::vector<CCompilationError>> GetErrors() const;
private:
    std::shared_ptr<const ASTree::CProgram> astRoot;
    ASTree::CTypeCheckerVisitor typeCheckerVisitor;
};

class CIrtBuildingPhase : public CCompilationPhase {
public:
    CIrtBuildingPhase( std::shared_ptr<const ASTree::CProgram> _astRoot,
            std::shared_ptr<const CSymbolTable> _symbolTable,
            bool _verbose = false )
    : CCompilationPhase( _verbose ), irtBuilderVisitor( _symbolTable, _verbose ), astRoot( _astRoot ) {}

    virtual void Run() override;

    std::shared_ptr<const std::unordered_map<std::string, std::shared_ptr<const IRTree::CStatement>>> MethodTrees() const;

    std::string ToDotLanguage( const std::string& methodName );
private:
    std::shared_ptr<const ASTree::CProgram> astRoot;

    std::shared_ptr<const std::unordered_map<std::string, std::shared_ptr<const IRTree::CStatement>>> methodTrees;
    ASTree::CIrtBuilderVisitor irtBuilderVisitor;

    IRTree::CDotLangVisitor dotLangVisitor;
    std::string dotLangTraversal;
};
