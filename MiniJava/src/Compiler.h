#pragma once

#include <string>

#include <CompilationPhase.h>

class CCompiler {
public:
    CCompiler( const std::string& _pathInputFile, const std::string& _pathOutputDir )
        : pathInputFile( _pathInputFile ), pathOutputDir( _pathOutputDir ) {}

    void Run();
private:
    std::string pathInputFile;
    std::string pathOutputDir;
};
