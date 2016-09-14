

#ifndef PRINT_VISITOR
#define PRINT_VISITOR

#include <Visitor.h>

class CPrintVisitor : public IVisitor {
    void Visit( CBinaryExpression* expression ) override;
    void Visit( CNumberExpression* expression ) override;
    void Visit( CIdExpression* expression ) override;
    void Visit( CPairListExpression* expression ) override;
    void Visit( CSingleElementListExpression* expression ) override;
    void Visit( CPrintStatement* statement ) override;
    void Visit( CAssignStatement* statement ) override;
    void Visit( CCompountStatement* statement ) override;
};

#endif
