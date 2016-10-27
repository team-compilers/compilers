#pragma once

#include <Expression.h>
#include <Visitor.h>
#include <VisitorTarget.h>
#include <vector>
#include <memory>


class CExpressionList : public IVisitorTarget {
public:

    CExpressionList() {}

    CExpressionList( const IExpression* expression ) { Add( expression ); }

    void Add( const IExpression* expression ) 
        { expressions.push_back( expression ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const IExpression> > expressions;
};
