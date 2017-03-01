#include <IRT/nodes/StatementList.h>

#include <cassert>

#include <IRT/nodes/Statement.h>

using namespace IRTree;

std::unique_ptr<const CStatementList> CStatementList::Clone() const {
    CStatementList* newList = new CStatementList();
    for ( auto it = statements.begin(); it != statements.end(); ++it ) {
        newList->Add( std::move( ( *it )->Clone() ) );
    }
    return std::move( std::unique_ptr<const CStatementList>( newList ) );
}

std::unique_ptr<const CStatementList> CStatementList::Canonize() const {
    // not implemented yet
    assert( false );
}
