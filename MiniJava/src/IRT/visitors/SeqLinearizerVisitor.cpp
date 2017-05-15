#include <IRT/visitors/SeqLinearizerVisitor.h>

using namespace IRTree;

std::unique_ptr<const CStatement> CSeqLinearizerVisitor::ResultTree() {
    return std::move( lastStatement );
}

void CSeqLinearizerVisitor::updateLastExpression( const CExpression* newLastExpression ) {
    lastExpression = std::unique_ptr<const CExpression>( newLastExpression );
}

void CSeqLinearizerVisitor::updateLastExpression( std::unique_ptr<const CExpression> newLastExpression ) {
    lastExpression = std::move( newLastExpression );
}

void CSeqLinearizerVisitor::updateLastStatement( const CStatement* newLastStatement ) {
    lastStatement = std::unique_ptr<const CStatement>( newLastStatement );
}

void CSeqLinearizerVisitor::updateLastStatement( std::unique_ptr<const CStatement> newLastStatement ) {
    lastStatement = std::move( newLastStatement );
}

void CSeqLinearizerVisitor::updateLastExpressionList( const CExpressionList* newLastExpressionList ) {
    lastExpressionList = std::unique_ptr<const CExpressionList>( newLastExpressionList );
}

void CSeqLinearizerVisitor::updateLastExpressionList( std::unique_ptr<const CExpressionList> newLastExpressionList ) {
    lastExpressionList = std::move( newLastExpressionList );
}

void CSeqLinearizerVisitor::saveResult( std::unique_ptr<const CStatement> result ) {
    if ( distanceToSeqStack.back() == 1 ) {
        statementStack.back()->push_back( std::move( result ) );
    } else {
        updateLastStatement( std::move( result ) );
    }
}

/*__________ Expressions __________*/

void CSeqLinearizerVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    updateLastExpression( std::move( expression->Clone() ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    updateLastExpression( std::move( expression->Clone() ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    updateLastExpression( std::move( expression->Clone() ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    expression->LeftOperand()->Accept( this );
    std::unique_ptr<const CExpression> leftOperand = std::move( lastExpression );
    expression->RightOperand()->Accept( this );
    std::unique_ptr<const CExpression> rightOperand = std::move( lastExpression );

    updateLastExpression(
        new CBinaryExpression(
            expression->Operation(),
            std::move( leftOperand ),
            std::move( rightOperand )
        )
    );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    expression->Address()->Accept( this );
    std::unique_ptr<const CExpression> addressExpression = std::move( lastExpression );

    updateLastExpression(
        new CMemExpression(
            std::move( addressExpression )
        )
    );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    expression->Function()->Accept( this );
    std::unique_ptr<const CExpression> functionExpression = std::move( lastExpression );
    expression->Arguments()->Accept( this );
    std::unique_ptr<const CExpressionList> argumentList = std::move( lastExpressionList );

    updateLastExpression(
        new CCallExpression(
            std::move( functionExpression ),
            std::move( argumentList )
        )
    );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CSeqLinearizerVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    statement->Expression()->Accept( this );
    std::unique_ptr<const CExpression> expression = std::move( lastExpression );

    std::unique_ptr<CStatement> result(
        new CExpStatement(
            std::move( expression )
        )
    );
    saveResult( std::move( result ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    statement->LeftOperand()->Accept( this );
    std::unique_ptr<const CExpression> leftOperand = std::move( lastExpression );
    statement->RightOperand()->Accept( this );
    std::unique_ptr<const CExpression> rightOperand = std::move( lastExpression );

    std::unique_ptr<CStatement> result(
        new CJumpConditionalStatement(
            statement->Operation(),
            std::move( leftOperand ),
            std::move( rightOperand ),
            statement->TrueLabel(),
            statement->FalseLabel()
        )
    );
    saveResult( std::move( result ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    std::unique_ptr<CStatement> result(
        new CJumpStatement( statement->Target() )
    );
    saveResult( std::move( result ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    std::unique_ptr<CStatement> result(
        new CLabelStatement( statement->Label() )
    );
    saveResult( std::move( result ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    statement->Destination()->Accept( this );
    std::unique_ptr<const CExpression> destination = std::move( lastExpression );
    statement->Source()->Accept( this );
    std::unique_ptr<const CExpression> source = std::move( lastExpression );

    std::unique_ptr<CStatement> result(
        new CMoveStatement(
            std::move( destination ),
            std::move( source )
        )
    );
    saveResult( std::move( result ) );

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    ++distanceToSeqStack.back();

    if ( distanceToSeqStack.back() > 1 ) {
        statementStack.emplace_back( new std::vector<std::unique_ptr<const CStatement>>() );
    }

    distanceToSeqStack.push_back( 0 );
    statement->LeftStatement()->Accept( this );
    statement->RightStatement()->Accept( this );

    distanceToSeqStack.pop_back();
    if ( distanceToSeqStack.back() > 1 ) {
        CStatementList* statementList = new CStatementList();
        for ( auto it = statementStack.back()->begin(); it != statementStack.back()->end(); ++it ) {
            statementList->Add( std::move( *it ) );
        }
        updateLastStatement( statementList );
        statementStack.pop_back();
    }

    --distanceToSeqStack.back();

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CSeqLinearizerVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_EXP );
    onNodeEnter( nodeName );

    CExpressionList* newList = new CExpressionList();

    const std::vector<std::unique_ptr<const CExpression>>& expressions = list->Expressions();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );
        newList->Add( std::move( lastExpression ) );
    }

    updateLastExpressionList( newList );

    onNodeExit( nodeName );
}

void CSeqLinearizerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}
