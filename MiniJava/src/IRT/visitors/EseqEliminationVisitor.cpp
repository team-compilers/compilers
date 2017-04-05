#include <IRT/visitors/DummyVisitor.h>

using namespace IRTree;

void CEseqEliminationVisitor::updateLastExpression( const CExpression* newLastExpression ) {
    lastExpression = std::unique_ptr<const CExpression>( newLastExpression );
}

void CEseqEliminationVisitor::updateLastExpression( std::unique_ptr<const CExpression> newLastExpression ) {
    lastExpression = std::move( newLastExpression );
}

void CEseqEliminationVisitor::updateLastExpressionList( const CExpressionList* newLastExpressionList ) {
    lastExpressionList = std::unique_ptr<const CExpressionList>( newLastExpressionList );
}

void CEseqEliminationVisitor::updateLastExpressionList( std::unique_ptr<const CExpressionList> newLastExpressionList ) {
    lastExpressionList = std::move( newLastExpressionList );
}

void CEseqEliminationVisitor::updateLastStatement( const CStatement* newLastStatement ) {
    lastStatement = std::unique_ptr<const CStatement>( newLastStatement );
}

void CEseqEliminationVisitor::updateLastStatement( std::unique_ptr<const CStatement> newLastStatement ) {
    lastStatement = std::move( newLastStatement );
}

void CEseqEliminationVisitor::updateLastStatementList( const CStatementList* newLastStatementList ) {
    lastStatementList = std::unique_ptr<const CStatementList>( newLastStatementList );
}

void CEseqEliminationVisitor::updateLastStatementList( std::unique_ptr<const CStatementList> newLastStatementList ) {
    lastStatementList = std::move( newLastStatementList );
}

/*__________ Expressions __________*/

void CEseqEliminationVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CEseqEliminationVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CEseqEliminationVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}
