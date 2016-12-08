#include <memory>
#include <string>
#include <astNodes/Program.h>

extern "C" FILE* yyin;
extern "C" FILE* yyout;

class CBisonParser {
public:
    CBisonParser(const std::string& _inputFilePath) : inputFilePath(_inputFilePath) {}
    std::unique_ptr<const AstTree::CProgram> buildAST( const std::string& inputFilePath );

private:
    std::string inputFilePath;
};
