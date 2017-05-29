#include <IRT/visitors/TraceFormationVisitor.h>

#include <cassert>

#include <IRT/Label.h>

using namespace IRTree;

std::unique_ptr<Synthesis::CTrace> CTraceFormationVisitor::Trace() {
    return std::move( trace );
}

std::string CTraceFormationVisitor::EndBlockLabelName() {
    return "epilogue";
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

    std::unique_ptr<Synthesis::CBlock> block;

    TNodeType previousNodeType = TNodeType::JUMP;

    const std::vector<std::unique_ptr<const CStatement>>& statements = list->Statements();
    for ( auto it = statements.begin(); it != statements.end(); ++it ) {
        ( *it )->Accept( this );

        if ( lastVisitedNodeType == TNodeType::LABEL || previousNodeType == TNodeType::JUMP ) {
            if ( block ) {
                finalizeBlockAndSave( std::move( block ), previousNodeType, false );
            }
            block = std::move( startNewBlock() );
        }
        block->Add( std::move( ( *it )->Clone() ) );

        previousNodeType = lastVisitedNodeType;
    }
    if ( block ) {
        finalizeBlockAndSave( std::move( block ), previousNodeType, true );
    }

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::finalizeBlockAndSave( std::unique_ptr<Synthesis::CBlock> block,
                                                   TNodeType previousNodeType,
                                                   bool isLastBlock ) {
    if ( previousNodeType != TNodeType::JUMP ) {
        CLabel jumpLabel = isLastBlock ? CLabel( EndBlockLabelName() ) : *lastVisitedLabel;
        block->Add(
            new CJumpStatement( jumpLabel )
        );
    }
    lastVisitedLabel = nullptr;
    trace->push_back( std::move( std::unique_ptr<const Synthesis::CBlock>( block.release() ) ) );
}

std::unique_ptr<Synthesis::CBlock> CTraceFormationVisitor::startNewBlock() {
    std::unique_ptr<Synthesis::CBlock> block( new Synthesis::CBlock() );
    if ( lastVisitedNodeType != TNodeType::LABEL ) {
        block->Add(
            new CLabelStatement( CLabel() )
        );
    }
    return std::move( block );
}
