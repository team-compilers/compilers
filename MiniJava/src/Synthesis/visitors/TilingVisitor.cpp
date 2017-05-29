#include <Synthesis/visitors/TilingVisitor.h>
#include <cassert>

using namespace Synthesis;

/*__________ Expressions __________*/

void CTilingVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    ApplyPatterns( expression );

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    ApplyPatterns(expression);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const IRTree::CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    ApplyPatterns(expression);


    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept(this);
    expression->RightOperand()->Accept(this);

    ApplyPatterns(expression);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    expression->Address()->Accept(this);

    ApplyPatterns(expression);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    expression->Function()->Accept(this);
    expression->Arguments()->Accept(this);

    ApplyPatterns(expression);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    assert( false );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CTilingVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    statement->Expression()->Accept(this);

    ApplyPatterns(statement);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    statement->LeftOperand()->Accept(this);
    statement->RightOperand()->Accept(this);

    ApplyPatterns(statement);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    ApplyPatterns(statement);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    ApplyPatterns(statement);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    statement->Source()->Accept(this);
    statement->Destination()->Accept(this);

    ApplyPatterns(statement);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    assert( false );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CTilingVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_EXP );
    onNodeEnter( nodeName );

    for(const auto& expression : list->Expressions()) {
        expression->Accept(this);
    }

    ApplyPatterns(list);

    onNodeExit( nodeName );
}

void CTilingVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    assert( false );

    onNodeExit( nodeName );
}
