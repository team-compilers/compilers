#include <fstream>
#include <memory>
#include <string>

#include <Program.h>
#include <DotLangVisitor.h>

#include <BisonParser.h>

std::string traverseAST( const CProgram* root, bool verbose ) {
    CDotLangVisitor visitor( verbose );
    visitor.Visit( root );
    return visitor.GetTraversalInDotLanguage();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::logic_error("Too few arguments provided");
    }
    const std::string inputFilePath(argv[1]);
    const std::string outputFilePath(argv[2]);
    CBisonParser parser(inputFilePath);
    std::unique_ptr<const CProgram> astRoot = parser.buildAST( inputFilePath );
    std::string traversal = traverseAST( astRoot.get(), false );

    std::ofstream outStream( outputFilePath );
    outStream << traversal;
    outStream.close();

    return 0;
}
