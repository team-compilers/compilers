#include <IRT/visitors/BlockRearrangeVisitor.h>

#include <cassert>

using namespace IRTree;

std::string CBlockRearrangeVisitor::LabelName() const {
    return labelName;
}

std::shared_ptr<std::string> CBlockRearrangeVisitor::JumpTargetLabelName() const {
    return jumpTargetLabelName;
}

/*__________ Expressions __________*/

void CBlockRearrangeVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CBlockRearrangeVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    jumpTargetLabelName = std::make_shared<std::string>( statement->FalseLabel().ToString() );

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    jumpTargetLabelName = std::make_shared<std::string>( statement->Target().ToString() );

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    labelName = statement->Label().ToString();

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CBlockRearrangeVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_EXP );
    onNodeEnter( nodeName );

    assert( false ); // such calls should never happen

    onNodeExit( nodeName );
}

void CBlockRearrangeVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    list->Statements().front()->Accept( this );
    list->Statements().back()->Accept( this );

    onNodeExit( nodeName );
}
