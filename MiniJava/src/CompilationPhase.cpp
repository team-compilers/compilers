#include <CompilationPhase.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <FileUtils.h>

#include <AST/visitors/DotLangVisitor.h>
#include <AST/visitors/GenerateCodeVisitor.h>
#include <AST/visitors/SymbolTableBuilderVisitor.h>
#include <AST/visitors/IrtBuilderVisitor.h>
#include <AST/visitors/TypeCheckerVisitor.h>

#include <IRT/visitors/DotLangVisitor.h>
#include <IRT/visitors/DoubleCallEliminationVisitor.h>
#include <IRT/visitors/SeqLinearizerVisitor.h>
#include <IRT/visitors/EseqEliminationVisitor.h>
#include <IRT/visitors/TraceFormationVisitor.h>

#include <Synthesis/Trace.h>
#include <Synthesis/visitors/DotLangVisitor.h>
#include <Synthesis/visitors/TilingVisitor.h>
#include <Synthesis/nodes/Commands.h>
#include <Synthesis/visitors/CommandEmitterVisitor.h>

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
        ASTree::CDotLangVisitor dotLangVisitor( verbose > 1 );
        astRoot->Accept( &dotLangVisitor );
        dotLangTraversal = dotLangVisitor.GetTraversalInDotLanguage();
    }
    return dotLangTraversal;
}

std::string CAstBuildingPhase::ToCode() {
    assert( astRoot );
    if ( codeGenerated.empty() ) {
        ASTree::CGenerateCodeVisitor generateCodeVisitor( verbose > 1 );
        astRoot->Accept( &generateCodeVisitor );
        codeGenerated = generateCodeVisitor.GetCode();
    }
    return codeGenerated;
}

void CSymbolTableBuildingPhase::Run() {
    ASTree::CSymbolTableBuilderVisitor symbolTableBuilderVisitor( verbose > 1 );
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
    ASTree::CTypeCheckerVisitor typeCheckerVisitor( symbolTable, verbose > 1 );
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
    ASTree::CIrtBuilderVisitor irtBuilderVisitor( symbolTable, verbose > 1 );
    irtBuilderVisitor.Visit( astRoot );
    methodTrees = std::move( irtBuilderVisitor.MethodTrees() );
}

void CIrtBuildingPhase::PrintResults( const std::string& pathOutputDir, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    const TMethodToIRTMap* methodTreesPtr = MethodTrees();
    for ( auto it = methodTreesPtr->begin(); it != methodTreesPtr->end(); ++it ) {
        const std::string& methodName = it->first;
        std::string dirPath = JoinPath( pathOutputDir, methodName );
        CreateDirectory( dirPath );

        std::fstream outputStream( JoinPath( dirPath, "irt" + extension), openMode );
        outputStream << ToDotLanguage( methodName ) << std::endl;
        outputStream.close();
    }
}

const TMethodToIRTMap* CIrtBuildingPhase::MethodTrees() const {
    assert( methodTrees );
    return methodTrees.get();
}

std::string CIrtBuildingPhase::ToDotLanguage( const std::string& methodName ) {
    assert( methodTrees );
    IRTree::CDotLangVisitor dotLangVisitor( verbose > 1 );
    methodTrees->at( methodName )->Accept( &dotLangVisitor );
    return dotLangVisitor.GetTraversalInDotLanguage();
}

void CIrtCanonizationPhase::Run() {
    for ( auto it = methodTrees->begin(); it != methodTrees->end(); ++it ) {
        IRTree::CDoubleCallEliminationVisitor callEliminationVisitor( verbose > 1 );
        it->second->Accept( &callEliminationVisitor );
        auto res = methodTreesWithoutDoubleCalls->emplace( it->first, std::move( callEliminationVisitor.ResultTree() ) );

        IRTree::CEseqEliminationVisitor eseqEliminationVisitor( verbose > 1 );
        res.first->second->Accept( &eseqEliminationVisitor );
        auto res2 = methodTreesWithoutEseqs->emplace( it->first, std::move( eseqEliminationVisitor.ResultTree() ) );

        IRTree::CSeqLinearizerVisitor seqLinearizerVisitor( verbose > 1 );
        res2.first->second->Accept( &seqLinearizerVisitor );
        methodTreesLinearized->emplace( it->first, std::move( seqLinearizerVisitor.ResultTree() ) );
    }
}

void CIrtCanonizationPhase::PrintResults( const std::string& pathOutputDir, const std::string& extension,
        const std::ios_base::openmode& openMode ) {
    if ( verbose > 0 ) {
        assert( methodTreesWithoutDoubleCalls );
        for ( auto it = methodTreesWithoutDoubleCalls->begin(); it != methodTreesWithoutDoubleCalls->end(); ++it ) {
            const std::string& methodName = it->first;
            std::string dirPath = JoinPath( pathOutputDir, methodName );
            CreateDirectory( dirPath );

            std::fstream outputStream( JoinPath( dirPath, "irtCanon1" + extension ), openMode );
            outputStream << ToDotLanguage( methodTreesWithoutDoubleCalls.get(), methodName ) << std::endl;
            outputStream.close();
        }

        assert( methodTreesWithoutEseqs );
        for ( auto it = methodTreesWithoutEseqs->begin(); it != methodTreesWithoutEseqs->end(); ++it ) {
            const std::string& methodName = it->first;
            std::string dirPath = JoinPath( pathOutputDir, methodName );
            CreateDirectory( dirPath );

            std::fstream outputStream( JoinPath( dirPath, "irtCanon2" + extension ), openMode );
            outputStream << ToDotLanguage( methodTreesWithoutEseqs.get(), methodName ) << std::endl;
            outputStream.close();
        }
    }

    assert( methodTreesLinearized );
    for ( auto it = methodTreesLinearized->begin(); it != methodTreesLinearized->end(); ++it ) {
        const std::string& methodName = it->first;
        std::string dirPath = JoinPath( pathOutputDir, methodName );
        CreateDirectory( dirPath );

        std::fstream outputStream( JoinPath( dirPath, "irtCanon" + extension ), openMode );
        outputStream << ToDotLanguage( methodTreesLinearized.get(), methodName ) << std::endl;
        outputStream.close();
    }
}

const TMethodToIRTMap* CIrtCanonizationPhase::MethodTrees() const {
    assert( methodTreesLinearized );
    return methodTreesLinearized.get();
}

std::string CIrtCanonizationPhase::ToDotLanguage( const TMethodToIRTMap* methodTreesMap,
                                                  const std::string& methodName ) {
    assert( methodTreesMap );
    IRTree::CDotLangVisitor dotLangVisitor( verbose > 1 );
    methodTreesMap->at( methodName )->Accept( &dotLangVisitor );
    return dotLangVisitor.GetTraversalInDotLanguage();
}

void CTraceFormationPhase::Run() {
    for ( auto it = methodTrees->begin(); it != methodTrees->end(); ++it ) {
        IRTree::CTraceFormationVisitor traceVisitor( verbose > 1 );
        it->second->Accept( &traceVisitor );

        std::unique_ptr<Synthesis::CTrace> trace = traceVisitor.Trace();
        trace = Synthesis::RearrangeBlocks( std::move( trace ) );
        std::unique_ptr<Synthesis::CTrace> traceConst( trace.release() );

        methodTraces->emplace( it->first, std::move( traceConst ) );
    }
}

void CTraceFormationPhase::PrintResults( const std::string& pathOutputDir,
                                         const std::string& extension,
                                         const std::ios_base::openmode& openMode ) {
    assert( methodTraces );
    for ( auto traceIt = methodTraces->begin(); traceIt != methodTraces->end(); ++traceIt ) {
        const std::string& methodName = traceIt->first;
        std::string dirPath = JoinPath( JoinPath( pathOutputDir, methodName ), "blocks");
        CreateDirectory( dirPath );

        int i = 0;
        for ( auto blockIt = traceIt->second->begin(); blockIt != traceIt->second->end(); ++blockIt, ++i ) {
            std::fstream outputStream( JoinPath( dirPath, std::to_string( i ) + extension ), openMode );
            outputStream << ToDotLanguage( blockIt->get() ) << std::endl;
            outputStream.close();
        }
    }
}

const TMethodToTraceMap* CTraceFormationPhase::MethodTraces() const {
    assert( methodTraces );
    return methodTraces.get();
}

std::string CTraceFormationPhase::ToDotLanguage( const Synthesis::CBlock* block ) {
    assert( block );
    IRTree::CDotLangVisitor dotLangVisitor( verbose > 1 );
    block->Accept( &dotLangVisitor );
    return dotLangVisitor.GetTraversalInDotLanguage();
}

void CTilingFormationPhase::Run() {
    for ( auto trace = methodTraces->begin(); trace != methodTraces->end(); ++trace ) {
        auto value = trace->second.get();
        for ( auto block = value->begin(); block != value->end(); ++block ) {
            for( const auto& statement : ( *block )->Statements() ) {
                Synthesis::CTilingVisitor* tilingVisitor = new Synthesis::CTilingVisitor( statement.get(), verbose > 1 );
                statement->Accept( tilingVisitor );
                commands[trace->first].emplace_back( tilingVisitor );
            }
        }
    }
}

std::string CTilingFormationPhase::ToDotLanguage( const Synthesis::CCommand* tree ) {
    assert( tree );
    Synthesis::CDotLangVisitor dotLangVisitor( verbose > 1 );
    tree->Accept( &dotLangVisitor );
    return dotLangVisitor.GetTraversalInDotLanguage();
}

void CTilingFormationPhase::PrintResults( const std::string& pathOutputDir,
                                          const std::string& extension,
                                          const std::ios_base::openmode& openMode ) {
    for ( auto methodIt = commands.begin(); methodIt != commands.end(); ++methodIt ) {
        const std::string& methodName = methodIt->first;
        std::string dirPath = JoinPath( JoinPath( pathOutputDir, methodName ), "commands");
        CreateDirectory( dirPath );

        int visitorIndex = 0;
        for ( auto visitorIt = methodIt->second.begin(); visitorIt != methodIt->second.end(); ++visitorIt, ++visitorIndex ) {
            const Synthesis::CCommand* command = ( *visitorIt )->Result();
            std::fstream outputStream( JoinPath( dirPath, std::to_string( visitorIndex ) + extension ), openMode );
            outputStream << ToDotLanguage( command ) << std::endl;
            outputStream.close();
        }
    }
}

const TMethodToCommandsList& CTilingFormationPhase::Commands() const {
    return commands;
}

void CAssemblyGenerationPhase::Run() {
    for ( auto methodIt = commands->begin(); methodIt != commands->end(); ++methodIt ) {
        for ( auto visitorIt = methodIt->second.begin(); visitorIt != methodIt->second.end(); ++visitorIt ) {
            Synthesis::CCommandEmitterVisitor commandEmitter( verbose > 1 );
            ( *visitorIt )->Result()->Accept( &commandEmitter );
            for( const auto& line : commandEmitter.Result() ) {
                std::stringstream builder;
                builder << line.Text();
                for( const auto& reg : line.Registers() ) {
                    builder << ' ' << reg;
                }
                codes.push_back( builder.str() );
            }
        }
    }
}

void CAssemblyGenerationPhase::PrintResults( const std::string& outputPath,
                                          const std::string& extension,
                                          const std::ios_base::openmode& openMode ) {
    std::fstream outputStream( outputPath + extension, openMode );
    for( const auto& line : codes ) {
        outputStream << line << std::endl;
    }
}
