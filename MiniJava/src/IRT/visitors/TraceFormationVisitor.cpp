#include <IRT/visitors/TraceFormationVisitor.h>

#include <cassert>

#include <IRT/Label.h>

using namespace IRTree;

std::unique_ptr<const Synthesis::CTrace> CTraceFormationVisitor::Trace() {
    return std::move( std::unique_ptr<const Synthesis::CTrace>( trace.release() ) );
}

/*__________ Expressions __________*/

void CTraceFormationVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CTraceFormationVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::JUMP;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::JUMP;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::LABEL;
    lastVisitedLabel = std::make_shared<CLabel>( statement->Label() );

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CTraceFormationVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_EXP );
    onNodeEnter( nodeName );

    lastVisitedNodeType = TNodeType::OTHER;

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    Synthesis::CBlock* block = new Synthesis::CBlock();

    TNodeType previousNodeType = TNodeType::JUMP;

    const std::vector<std::unique_ptr<const CStatement>>& statements = list->Statements();
    for ( auto it = statements.begin(); it != statements.end(); ++it ) {
        ( *it )->Accept( this );

        if ( lastVisitedNodeType == TNodeType::LABEL || previousNodeType == TNodeType::JUMP ) {
            if ( previousNodeType != TNodeType::JUMP ) {
                // current: LABEL, previous: not JUMP
                // add jump to the current block ...
                block->Add(
                    new CJumpStatement( *lastVisitedLabel )
                );
                lastVisitedLabel = nullptr;
            }
            // ... and create a new block
            trace->emplace_back( block );
            block = new Synthesis::CBlock();
            if ( lastVisitedNodeType != TNodeType::LABEL ) {
                // current: not LABEL, previous: JUMP
                // add label to current block
                block->Add(
                    new CLabelStatement( CLabel() )
                );
            }
        }
        block->Add( std::move( ( *it )->Clone() ) );

        previousNodeType = lastVisitedNodeType;
    }

    onNodeExit( nodeName );
}
