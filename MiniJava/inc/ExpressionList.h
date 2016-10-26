#pragma once

#include <Expression.h>
#include <Visition.h>

class CExpressionList : public IVisitorTarget {
public:

    CExpressionList() {}

    CExpressionList( const IExpression* expression ) { Add( expression ); }

    Add( const IExpression* expression ) 
        { expressions.push_back( expression ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const IExpression> > expressions;
}