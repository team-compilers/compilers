#include <IRT/nodes/ExpressionList.h>

#include <cassert>

#include <IRT/nodes/Expression.h>

using namespace IRTree;

std::unique_ptr<const CExpressionList> CExpressionList::Clone() const {
    CExpressionList* newList = new CExpressionList();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        newList->Add( std::move( ( *it )->Clone() ) );
    }
    return std::move( std::unique_ptr<const CExpressionList>( newList ) );
}
