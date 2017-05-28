#include <Compiler.h>
#include <FileUtils.h>

#include <fstream>

void CCompiler::Run() {
    CAstBuildingPhase astBuilding( pathInputFile );
    astBuilding.Run();
    astBuilding.PrintResults( JoinPath( pathOutputDir, "ast" ), ".gv" );
    astBuilding.PrintCodeGenerated( JoinPath( pathOutputDir, "code" ), ".java" );
    
    CSymbolTableBuildingPhase symbolTableBuilding( astBuilding.GetAstRoot() );
    symbolTableBuilding.Run();
    symbolTableBuilding.PrintResults( JoinPath( pathOutputDir, "errors" ), ".txt" );

    if ( !symbolTableBuilding.GetErrors()->empty() ) {
        return;
    }

    CTypeCheckingPhase typeChecking( astBuilding.GetAstRoot(), symbolTableBuilding.GetSymbolTable() );
    typeChecking.Run();
    typeChecking.PrintResults( JoinPath( pathOutputDir, "errors" ), ".txt", std::fstream::app );

    if ( !typeChecking.GetErrors()->empty() ) {
        return;
    }

    CIrtBuildingPhase irtBuilding( astBuilding.GetAstRoot(), symbolTableBuilding.GetSymbolTable() );
    irtBuilding.Run();
    irtBuilding.PrintResults( pathOutputDir, ".gv" );

    CIrtCanonizationPhase irtCanonization( irtBuilding.MethodTrees() );
    irtCanonization.Run();
    irtCanonization.PrintResults( pathOutputDir, ".gv" );

    CTraceFormationPhase traceFormation( irtCanonization.MethodTrees() );
    traceFormation.Run();
    traceFormation.PrintResults( pathOutputDir, ".gv" );

//    CTilingFormationPhase tilingFormation( traceFormation.MethodTraces() );
//    tilingFormation.Run();
}
