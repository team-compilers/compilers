#pragma once

#include <string>

#include <CompilationPhase.h>

class CCompilator {
public:
    CCompilator( const std::string& _pathInputFile, const std::string& _pathOutputDir )
        : pathInputFile( _pathInputFile ), pathOutputDir( _pathOutputDir ) {}

    void Run();
private:
    std::string pathInputFile;
    std::string pathOutputDir;
};
