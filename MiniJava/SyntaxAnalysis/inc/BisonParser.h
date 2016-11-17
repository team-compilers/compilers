#include <memory>
#include <string>
#include <Program.h>

extern "C" FILE* yyin;
extern "C" FILE* yyout;

class CBisonParser {
public:
    CBisonParser(const std::string& _inputFilePath) : inputFilePath(_inputFilePath) {}
    std::unique_ptr<const CProgram> buildAST( const std::string& inputFilePath );

private:
    std::string inputFilePath;
};
