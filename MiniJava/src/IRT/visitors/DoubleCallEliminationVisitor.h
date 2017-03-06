#pragma once

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

class CDoubleCallEliminationVisitor : public CVisitor {
public:
    CDoubleCallEliminationVisitor( bool _verbose = false ) : CVisitor( _verbose ) {}
    ~CDoubleCallEliminationVisitor() {}

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

    void updateLastExpressionList( const CExpressionList* newLastExpressionList );
    void updateLastExpressionList( std::unique_ptr<const CExpressionList> newLastExpressionList );

    void updateLastStatement( const CStatement* newLastStatement );
    void updateLastStatement( std::unique_ptr<const CStatement> newLastStatement );

    void updateLastStatementList( const CStatementList* newLastStatementList );
    void updateLastStatementList( std::unique_ptr<const CStatementList> newLastStatementList );

    std::unique_ptr<const CExpression> lastExpression;
    std::unique_ptr<const CStatement> lastStatement;
    std::unique_ptr<const CExpressionList> lastExpressionList;
    std::unique_ptr<const CStatementList> lastStatementList;
};

}
