#pragma once

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>

using namespace IRTree;

namespace Synthesis {

class CTilingVisitor : public CVisitor {
public:
    CTilingVisitor( bool _verbose = false ) : CVisitor( _verbose ) {}
    ~CTilingVisitor() {}

    // Visitors for different node types.
    void Visit( const CConstExpression* expression ) override;
    void Visit( const CNameExpression* expression ) override;
    void Visit( const CTempExpression* expression ) override;
    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CMemExpression* expression ) override;
    void Visit( const CCallExpression* expression ) override;
    void Visit( const CEseqExpression* expression ) override;

    void Visit( const CExpStatement* statement ) override;
    void Visit( const CJumpConditionalStatement* statement ) override;
    void Visit( const CJumpStatement* statement ) override;
    void Visit( const CLabelStatement* statement ) override;
    void Visit( const CMoveStatement* statement ) override;
    void Visit( const CSeqStatement* statement ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;
};

}
