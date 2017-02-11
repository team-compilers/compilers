#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

namespace ASTree {

class CExpression;

class CExpressionList : public CVisitorTarget {
public:
    CExpressionList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    CExpressionList( const CExpression* expression, const CLocation& _location )
        : CVisitorTarget( _location ) { Add( expression ); }

    void Add( const CExpression* expression )
        { expressions.emplace_back( expression ); }

    void Add( std::unique_ptr<const CExpression> expression )
        { expressions.push_back( std::move( expression ) ); }

    const std::vector< std::unique_ptr<const CExpression> >& Expressions() const { return expressions; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CExpression> > expressions;
};

}
