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
    CExpressionList( std::unique_ptr<const CExpression> expression ) { Add( std::move( expression ) ); }

    void Add( const CExpression* expression ) { expressions.emplace_back( expression ); }
    void Add( std::unique_ptr<const CExpression> expression ) { expressions.push_back( std::move( expression ) ); }
    const std::vector< std::unique_ptr<const CExpression> >& Expressions() const { return expressions; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpressionList> Clone() const;

private:
    std::vector<std::unique_ptr<const CExpression>> expressions;
};

}
