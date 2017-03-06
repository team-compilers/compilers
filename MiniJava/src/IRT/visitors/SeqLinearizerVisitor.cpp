#include <IRT/visitors/SeqLinearizerVisitor.h>

using namespace IRTree;

/*__________ Expressions __________*/

void CSeqLinearizerVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CSeqLinearizerVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();
    // if ( isAddToLeftStack.back() ) {
    //     dequeStack.back().push_front( std::move( statement->Clone() ) );
    // } else {
    //     dequeStack.back().push_back( std::move( statement->Clone() ) );
    // }

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    // if ( distanceToSeqStack.back() > 1 ) {
    //     dequeStack.push_back( std::deque<std::unique_ptr<const CStatement>>() );
    // }

    // isAddToLeftStack.push_back( true );
    // statement->LeftStatement()->Accept( this );
    // isAddToLeftStack.pop_back();

    // isAddToLeftStack.push_back( false );
    // statement->RightStatement()->Accept( this );
    // isAddToLeftStack.pop_back();

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CSeqLinearizerVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}
