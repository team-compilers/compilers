#pragma once

#include <memory>
#include <vector>

#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class CExpression;

class CExpressionList : public IVisitorTarget {
public:
    CExpressionList() = default;

    CExpressionList( const CExpression* expression ) { Add( expression ); }

    void Add( const CExpression* expression )
        { expressions.emplace_back( expression ); }

    const std::vector< std::unique_ptr<const CExpression> >& Expressions() const { return expressions; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CExpression> > expressions;
};

}
