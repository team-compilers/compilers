#pragma once

#include <memory>

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>
#include <IRT/Label.h>

#include <Synthesis/Trace.h>

namespace IRTree {

class CTraceFormationVisitor : public CVisitor {
public:
    CTraceFormationVisitor( bool _verbose = false )
        : CVisitor( _verbose ), lastVisitedNodeType( TNodeType::OTHER ),
          trace( new Synthesis::CTrace() ) {}
    ~CTraceFormationVisitor() {}

    std::unique_ptr<Synthesis::CTrace> Trace();
    static std::string EndBlockLabelName();

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
    enum class TNodeType : char {
        LABEL, JUMP, OTHER
    };

    void finalizeBlockAndSave(std::unique_ptr<Synthesis::CBlock> block,
                              TNodeType previousNodeType,
                              bool isLastBlock);
    std::unique_ptr<Synthesis::CBlock> startNewBlock();

    std::shared_ptr<CLabel> lastVisitedLabel;
    TNodeType lastVisitedNodeType;
    std::unique_ptr<Synthesis::CTrace> trace;
};

}
