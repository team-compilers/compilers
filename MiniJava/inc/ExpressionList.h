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

    const std::vector< std::unique_ptr<const IExpression> >& Expressions() const { return expressions; }
    
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const IExpression> > expressions;
};
