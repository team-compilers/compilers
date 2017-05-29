#pragma once

#include <Synthesis/visitors/Visitor.h>
#include <Synthesis/AssemblyCommand.h>
#include <vector>
#include <string>
#include <map>

namespace Synthesis {

class CCommandEmitterVisitor : public Synthesis::CVisitor {
public:
    virtual ~CCommandEmitterVisitor() {}

    void Visit( const CConditionalJumpCommand* command ) override;
    void Visit( const CJumpCommand* command ) override;
    void Visit( const CCallFunctionCommand* command ) override;
    void Visit( const CLabelDeclarationCommand* command ) override;
    void Visit( const CMoveRegisterCommand* command ) override;
    void Visit( const CTempExpression* expression ) override;
    void Visit( const CAddCommand* expression ) override;
    void Visit( const CAddConstCommand* expression ) override;
    void Visit( const CSubCommand* expression ) override;
    void Visit( const CSubConstCommand* expression ) override;
    void Visit( const CMultCommand* expression ) override;
    void Visit( const CDivCommand* expression ) override;
    void Visit( const CLoadCommand* expression ) override;
    void Visit( const CStoreCommand* expression ) override;
    void Visit( const CNullExpression* expression ) override;
private:
    std::string lastRegisterValue;
    std::vector<CAssemblyCommand> code;

    std::map<std::string, std::string> tempToRegister;
};

}; // Synthesis