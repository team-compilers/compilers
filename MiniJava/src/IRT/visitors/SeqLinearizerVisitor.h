#pragma once

#include <cassert>
#include <deque>
#include <limits>
#include <vector>

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

class CSeqLinearizerVisitor : public CVisitor {
public:
    CSeqLinearizerVisitor( bool _verbose = false ) : CVisitor( _verbose ), distanceToSeqStack( 1, std::numeric_limits<int>::max() - 1 ) {}
    ~CSeqLinearizerVisitor() {}

    std::unique_ptr<const CStatement> ResultTree();

    // Visitors for different node types.
    void Visit( const CConstExpression* expression ) override;
    void Visit( const CNameExpression* expression ) override;
    void Visit( const CTempExpression* expression ) override;
    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CMemExpression* expression ) override;
    void Visit( const CCallExpression* expression ) override;
    void Visit( const CEseqExpression* expression ) override;

    void Visit( const CExpStatement* expression ) override;
    void Visit( const CJumpConditionalStatement* expression ) override;
    void Visit( const CJumpStatement* expression ) override;
    void Visit( const CLabelStatement* expression ) override;
    void Visit( const CMoveStatement* expression ) override;
    void Visit( const CSeqStatement* expression ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;
private:
    void updateLastExpression( const CExpression* newLastExpression );
    void updateLastExpression( std::unique_ptr<const CExpression> newLastExpression );

    void updateLastStatement( const CStatement* newLastStatement );
    void updateLastStatement( std::unique_ptr<const CStatement> newLastStatement );

    void updateLastExpressionList( const CExpressionList* newLastExpressionList );
    void updateLastExpressionList( std::unique_ptr<const CExpressionList> newLastExpressionList );

    void saveResult( std::unique_ptr<const CStatement> result );

    std::vector<std::unique_ptr<std::vector<std::unique_ptr<const CStatement>>>> statementStack; // stack of statements
    std::vector<int> distanceToSeqStack;
    std::unique_ptr<const CStatement> lastStatement;
    std::unique_ptr<const CExpression> lastExpression;
    std::unique_ptr<const CExpressionList> lastExpressionList;
};

}
