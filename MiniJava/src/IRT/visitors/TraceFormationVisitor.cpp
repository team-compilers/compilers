#include <IRT/visitors/TraceFormationVisitor.h>

using namespace IRTree;

/*__________ Expressions __________*/

void CTraceFormationVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CTraceFormationVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CTraceFormationVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTraceFormationVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}