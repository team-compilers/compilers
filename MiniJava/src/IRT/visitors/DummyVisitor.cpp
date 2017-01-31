#include <IRT/visitors/DummyVisitor.h>

using namespace IRTree;

/*__________ Expressions __________*/

void CDummyVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CDummyVisitor::Visit( const CExpStatement* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CJumpConditionalStatement* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CJumpStatement* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CLabelStatement* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CMoveStatement* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CSeqStatement* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CDummyVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CDummyVisitor::Visit( const CLabelList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LABEL_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}
