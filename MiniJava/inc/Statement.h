// Author: Alexey Zhuravlev
// Description: Statement Interface and it's implementations

#pragma once

#include <Visitor.h>
#include <VisitorTarget.h>
#include <Expression.h>

class IStatement : public IVisitorTarget {
};

struct CCompoundStatement : public IStatement {
    IStatement* First; // First statement of the compound
    IStatement* Second; // Second statement of the compound

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }
};

struct CAssignStatement : public IStatement {
    CIdExpression* LeftValue; // Left part of assign statement
    IExpression* RightValue; // Right part of assign statement

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }
};

struct CPrintStatement : public IStatement {
    IListExpression* PrintList; // List of expressions to print

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }
};
