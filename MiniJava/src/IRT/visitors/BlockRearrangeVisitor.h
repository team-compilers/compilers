#pragma once

#include <memory>
#include <string>

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

class CBlockRearrangeVisitor : public CVisitor {
public:
    CBlockRearrangeVisitor( bool _verbose = false ) : CVisitor( _verbose ) {}
    ~CBlockRearrangeVisitor() {}

    std::string LabelName() const;
    std::shared_ptr<std::string> JumpTargetLabelName() const;

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
    std::string labelName;
    std::shared_ptr<std::string> jumpTargetLabelName;
};

}
