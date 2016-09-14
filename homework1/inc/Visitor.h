// Author: Alexey Zhuravlev
// Description: Visitor interface

#ifndef VISITOR
#define VISITOR

#include <Expression.h>
#include <Statement.h>

class IVisitor {
    public:
        virtual void Visit( CBinaryExpression* expression ) = 0;
        virtual void Visit( CNumberExpression* expression ) = 0;
};

#endif
