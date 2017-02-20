#pragma once

#include <fstream>
#include <memory>
#include <string>

#include <AST/nodes/Program.h>
#include <CompilationError.h>
#include <SymbolTable.h>
#include <IRT/nodes/Statement.h>

class CCompilationPhase {
public:
    CCompilationPhase( bool _verbose = false ) : verbose( _verbose ) {}
    virtual ~CCompilationPhase() = default;

    virtual void Run() = 0;
    virtual void PrintResults(
        const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) = 0;
protected:
    bool verbose;
};

class CAstBuildingPhase : public CCompilationPhase {
public:
    CAstBuildingPhase( const std::string& _pathInputFile, bool _verbose = false )
        : CCompilationPhase( _verbose ), pathInputFile( _pathInputFile ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    void PrintCodeGenerated( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out );

    const ASTree::CProgram* GetAstRoot() const;
    std::string ToDotLanguage();
    std::string ToCode();
private:
    // parameters
    std::string pathInputFile;

    // results
    std::unique_ptr<const ASTree::CProgram> astRoot;
    std::string dotLangTraversal;
    std::string codeGenerated;
};

class CSymbolTableBuildingPhase : public CCompilationPhase {
public:
    CSymbolTableBuildingPhase( const ASTree::CProgram* _astRoot, bool _verbose = false )
        : CCompilationPhase( _verbose ), astRoot( _astRoot ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    const CSymbolTable* GetSymbolTable() const;
    const std::vector<CCompilationError>* GetErrors() const;
private:
    // parameters
    const ASTree::CProgram* astRoot;

    // results
    std::unique_ptr<const CSymbolTable> symbolTable;
    std::unique_ptr<const std::vector<CCompilationError>> errors;
};

class CTypeCheckingPhase : public CCompilationPhase {
public:
    CTypeCheckingPhase( const ASTree::CProgram* _astRoot,
            const CSymbolTable* _symbolTable,
            bool _verbose = false )
        : CCompilationPhase( _verbose ), astRoot( _astRoot ), symbolTable( _symbolTable ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    const std::vector<CCompilationError>* GetErrors() const;
private:
    // parameters
    const ASTree::CProgram* astRoot;
    const CSymbolTable* symbolTable;

    // results
    std::unique_ptr<const std::vector<CCompilationError>> errors;
};

using TMethodToIRTMap = std::unordered_map<std::string, std::unique_ptr<const IRTree::CStatement>>;

class CIrtBuildingPhase : public CCompilationPhase {
public:

    CIrtBuildingPhase( const ASTree::CProgram* _astRoot,
            const CSymbolTable* _symbolTable,
            bool _verbose = false )
    : CCompilationPhase( _verbose ), astRoot( _astRoot ), symbolTable( _symbolTable ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    const TMethodToIRTMap* MethodTrees() const;

    std::string ToDotLanguage( const std::string& methodName );
private:
    //parameters
    const ASTree::CProgram* astRoot;
    const CSymbolTable* symbolTable;

    // results
    std::unique_ptr<const TMethodToIRTMap> methodTrees;
    std::string dotLangTraversal;
};

class CIrtCanonizationPhase : CCompilationPhase {
public:
    CIrtCanonizationPhase( const TMethodToIRTMap* _methodTrees, bool _verbose = false )
        : CCompilationPhase( _verbose ), methodTrees( _methodTrees ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;
private:
    // parameters
    const TMethodToIRTMap* methodTrees;
};
