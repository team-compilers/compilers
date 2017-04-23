#include <Compiler.h>

#include <fstream>

void CCompiler::Run() {
    CAstBuildingPhase astBuilding( pathInputFile );
    astBuilding.Run();
    astBuilding.PrintResults( pathOutputDir + "/ast", ".gv" );
    astBuilding.PrintCodeGenerated( pathOutputDir + "/code", ".java" );
    
    CSymbolTableBuildingPhase symbolTableBuilding( astBuilding.GetAstRoot() );
    symbolTableBuilding.Run();
    symbolTableBuilding.PrintResults( pathOutputDir + "/errors", ".txt" );

    if ( !symbolTableBuilding.GetErrors()->empty() ) {
        return;
    }

    CTypeCheckingPhase typeChecking( astBuilding.GetAstRoot(), symbolTableBuilding.GetSymbolTable() );
    typeChecking.Run();
    typeChecking.PrintResults( pathOutputDir + "/errors", ".txt", std::fstream::app );

    if ( !typeChecking.GetErrors()->empty() ) {
        return;
    }

    CIrtBuildingPhase irtBuilding( astBuilding.GetAstRoot(), symbolTableBuilding.GetSymbolTable() );
    irtBuilding.Run();
    irtBuilding.PrintResults( pathOutputDir + "/irt_", ".gv" );

    CIrtCanonizationPhase irtCanonization( irtBuilding.MethodTrees() );
    irtCanonization.Run();
    irtCanonization.PrintResults( pathOutputDir + "/irtCanon", ".gv" );

    CTraceFormationPhase traceFormation( irtCanonization.MethodTrees() );
    traceFormation.Run();
}
