#pragma once

#include <Synthesis/visitors/Visitor.h>

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
    void Visit( const CMoveCommand* expression ) override;
    void Visit( const CNullExpression* expression ) override;
private:

};

}; // Synthesis