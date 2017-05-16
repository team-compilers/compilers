#pragma once

#include <fstream>
#include <memory>
#include <string>

#include <AST/nodes/Program.h>
#include <CompilationError.h>
#include <SymbolTable.h>
#include <IRT/nodes/Statement.h>
#include <Synthesis/Trace.h>
#include <Synthesis/visitors/TilingVisitor.h>

class CCompilationPhase {
public:
    CCompilationPhase( int _verbose = 0 ) : verbose( _verbose ) {}
    virtual ~CCompilationPhase() = default;

    virtual void Run() = 0;
    virtual void PrintResults(
        const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) = 0;

protected:
    int verbose;
};

class CAstBuildingPhase : public CCompilationPhase {
public:
    CAstBuildingPhase( const std::string& _pathInputFile, int _verbose = 0 )
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
    CSymbolTableBuildingPhase( const ASTree::CProgram* _astRoot, int _verbose = 0 )
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
            int _verbose = 0 )
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
            int _verbose = 0 )
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

class CIrtCanonizationPhase : public CCompilationPhase {
public:
    CIrtCanonizationPhase( const TMethodToIRTMap* _methodTrees, int _verbose = 0 )
        : CCompilationPhase( _verbose ), methodTrees( _methodTrees ),
            methodTreesWithoutDoubleCalls( new TMethodToIRTMap() ),
            methodTreesWithoutEseqs( new TMethodToIRTMap() ),
            methodTreesLinearized( new TMethodToIRTMap() ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    const TMethodToIRTMap* MethodTrees() const;

    std::string ToDotLanguage( const TMethodToIRTMap* methodTreesMap, const std::string& methodName );

private:
    // parameters
    const TMethodToIRTMap* methodTrees;

    // results
    std::unique_ptr<TMethodToIRTMap> methodTreesWithoutDoubleCalls;
    std::unique_ptr<TMethodToIRTMap> methodTreesWithoutEseqs;
    std::unique_ptr<TMethodToIRTMap> methodTreesLinearized;
};

using TMethodToTraceMap = std::unordered_map<std::string, std::unique_ptr<const Synthesis::CTrace>>;

class CTraceFormationPhase : public CCompilationPhase {
public:
    CTraceFormationPhase( const TMethodToIRTMap* _methodTrees, int _verbose = 0 )
        : CCompilationPhase( _verbose ), methodTrees( _methodTrees ),
            methodTraces( new TMethodToTraceMap() ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    const TMethodToTraceMap* MethodTraces() const;

    std::string ToDotLanguage( const Synthesis::CBlock* block );

private:
    // parameters
    const TMethodToIRTMap* methodTrees;

    // results
    std::unique_ptr<TMethodToTraceMap> methodTraces;
};

using TMethodToCommandsList = std::unordered_map<std::string,
    std::vector<
        std::unique_ptr<const Synthesis::CTilingVisitor>
    >
>;

class CTilingFormationPhase : public CCompilationPhase {
public:
    CTilingFormationPhase( const TMethodToTraceMap* _methodTraces, int _verbose = 0 )
        : CCompilationPhase( _verbose ), methodTraces( _methodTraces ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

    const TMethodToCommandsList& Commands() const;
    std::string ToDotLanguage( const Synthesis::CCommand* tree );

private:
    const TMethodToTraceMap* methodTraces;

    TMethodToCommandsList commands;
};

class CAssemblyGenerationPhase : public CCompilationPhase {
public:
    CAssemblyGenerationPhase( const TMethodToCommandsList* _commands, int _verbose = 0 )
        : CCompilationPhase( _verbose ), commands( _commands ) {}

    virtual void Run() override;
    virtual void PrintResults( const std::string& pathOutputFile, const std::string& extension,
        const std::ios_base::openmode& openMode = std::fstream::out ) override;

private:
    const TMethodToCommandsList* commands;

    std::vector<std::string> codes;
};
