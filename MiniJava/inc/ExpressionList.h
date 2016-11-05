#pragma once

#include <Visitor.h>
#include <VisitorTarget.h>
#include <vector>
#include <memory>

class IExpression;

class CExpressionList : public IVisitorTarget {
public:

    CExpressionList() = default;

    CExpressionList( const IExpression* expression ) { Add( expression ); }

    void Add( const IExpression* expression ) 
        { expressions.emplace_back( expression ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const IExpression> > expressions;
};
