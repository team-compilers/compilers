#pragma once

#include <Expression.h>

class CExpressionList : public IVisitorTarget {
public:

    CExpressionList() {}

    CExpressionList( const IExpression* expression ) { Add( expression ); }

    Add( const IExpression* expression ) 
        { expressions.push_back( expression ); }
private:
    std::vector< std::unique_ptr<const IExpression> > expressions;
}
