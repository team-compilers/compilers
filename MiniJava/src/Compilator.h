#pragma once

#include <string>

class CCompilator {
public:
    CCompilator( const std::string& _fileNameInput, const std::string& _dirNameOutput )
        : fileNameInput( _fileNameInput ), dirNameOutput( _dirNameOutput ) {}

    void Run();
private:
    std::string fileNameInput;
    std::string dirNameOutput;
};
