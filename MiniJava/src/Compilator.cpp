#include <Compilator.h>

#include <fstream>

void CCompilator::Run() {
    CAstBuildingPhase astBuilding( pathInputFile );
    astBuilding.Run();
    astBuilding.PrintResults( pathOutputDir + "/ast.gv" );
    
    CSymbolTableBuildingPhase symbolTableBuilding( astBuilding.GetAstRoot() );
    symbolTableBuilding.Run();
    symbolTableBuilding.PrintResults( pathOutputDir + "/errors.txt" );

    if ( !symbolTableBuilding.GetErrors()->empty() ) {
        return;
    }

    CTypeCheckingPhase typeChecking( astBuilding.GetAstRoot(), symbolTableBuilding.GetSymbolTable() );
    typeChecking.Run();
    typeChecking.PrintResults( pathOutputDir + "/errors.txt", std::fstream::app );

    if ( !typeChecking.GetErrors()->empty() ) {
        return;
    }
}
