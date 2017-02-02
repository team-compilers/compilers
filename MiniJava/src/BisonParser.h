#include <memory>
#include <string>
#include <AST/nodes/Program.h>

extern "C" FILE* yyin;
extern "C" FILE* yyout;

class CBisonParser {
public:
    CBisonParser(const std::string& _inputFilePath) : inputFilePath(_inputFilePath) {}
    const ASTree::CProgram* buildAST( const std::string& inputFilePath );

private:
    std::string inputFilePath;
};
