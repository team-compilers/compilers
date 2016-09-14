// Author: Alexey Zhuravlev
// Description: Visitor interface

#ifndef VISITOR
#define VISITOR

struct CBinaryExpression;
struct CNumberExpression;
struct CIdExpression;
struct CPairListExpression;
struct CSingleElementListExpression;
struct CPrintStatement;
struct CAssignStatement;
struct CCompountStatement;

class IVisitor {
public:
    virtual void Visit( CNumberExpression* expression ) = 0;
    virtual void Visit( CBinaryExpression* expression ) = 0;
    virtual void Visit( CIdExpression* expression ) = 0;
    virtual void Visit( CPairListExpression* expression ) = 0;
    virtual void Visit( CSingleElementListExpression* expression ) = 0;
    virtual void Visit( CPrintStatement* statement ) = 0;
    virtual void Visit( CAssignStatement* statement ) = 0;
    virtual void Visit( CCompountStatement* statement ) = 0;
};

#endif
