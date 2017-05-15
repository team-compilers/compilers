#include <IRT/visitors/DoubleCallEliminationVisitor.h>
#include <IRT/Temporary.h>

using namespace IRTree;

std::unique_ptr<const CStatement> CDoubleCallEliminationVisitor::ResultTree() {
    return std::move( lastStatement );
}

void CDoubleCallEliminationVisitor::updateLastExpression( const CExpression* newLastExpression ) {
    lastExpression = std::unique_ptr<const CExpression>( newLastExpression );
}

void CDoubleCallEliminationVisitor::updateLastExpression( std::unique_ptr<const CExpression> newLastExpression ) {
    lastExpression = std::move( newLastExpression );
}

void CDoubleCallEliminationVisitor::updateLastExpressionList( const CExpressionList* newLastExpressionList ) {
    lastExpressionList = std::unique_ptr<const CExpressionList>( newLastExpressionList );
}

void CDoubleCallEliminationVisitor::updateLastExpressionList( std::unique_ptr<const CExpressionList> newLastExpressionList ) {
    lastExpressionList = std::move( newLastExpressionList );
}

void CDoubleCallEliminationVisitor::updateLastStatement( const CStatement* newLastStatement ) {
    lastStatement = std::unique_ptr<const CStatement>( newLastStatement );
}

void CDoubleCallEliminationVisitor::updateLastStatement( std::unique_ptr<const CStatement> newLastStatement ) {
    lastStatement = std::move( newLastStatement );
}

void CDoubleCallEliminationVisitor::updateLastStatementList( const CStatementList* newLastStatementList ) {
    lastStatementList = std::unique_ptr<const CStatementList>( newLastStatementList );
}

void CDoubleCallEliminationVisitor::updateLastStatementList( std::unique_ptr<const CStatementList> newLastStatementList ) {
    lastStatementList = std::move( newLastStatementList );
}


/*__________ Expressions __________*/

void CDoubleCallEliminationVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    updateLastExpression(
        new CConstExpression( expression->Value() )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    updateLastExpression(
        new CNameExpression( expression->Label() )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    updateLastExpression(
        new CTempExpression( expression->Temporary() )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept( this );
    std::unique_ptr<const CExpression> expressionLeft = std::move( lastExpression );

    expression->RightOperand()->Accept( this );
    std::unique_ptr<const CExpression> expressionRight = std::move( lastExpression );

    updateLastExpression(
        new CBinaryExpression(
            expression->Operation(),
            std::move( expressionLeft ),
            std::move( expressionRight )
        )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    expression->Address()->Accept( this );
    std::unique_ptr<const CExpression> addressExpression = std::move( lastExpression );

    updateLastExpression(
        new CMemExpression( std::move( addressExpression ) )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );

    expression->Function()->Accept( this );
    std::unique_ptr<const CExpression> functionExpression = std::move( lastExpression );
    expression->Arguments()->Accept( this );
    std::unique_ptr<const CExpressionList> argumentsList = std::move( lastExpressionList );

    CTemp temp;
    updateLastExpression(
        new CEseqExpression(
            new CMoveStatement(
                new CTempExpression( temp ),
                new CCallExpression(
                    std::move( functionExpression ),
                    std::move( argumentsList )
                )
            ),
            new CTempExpression( temp )
        )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    expression->Statement()->Accept( this );
    expression->Expression()->Accept( this );

    updateLastExpression(
        new CEseqExpression(
            std::move( lastStatement ),
            std::move( lastExpression )
        )
    );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CDoubleCallEliminationVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    statement->Expression()->Accept( this );
    std::unique_ptr<const CExpression> expression = std::move( lastExpression );

    updateLastStatement(
        new CExpStatement( std::move( expression ) )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    statement->LeftOperand()->Accept( this );
    std::unique_ptr<const CExpression> expressionLeft = std::move( lastExpression );

    statement->RightOperand()->Accept( this );
    std::unique_ptr<const CExpression> expressionRight = std::move( lastExpression );

    updateLastStatement(
        new CJumpConditionalStatement(
            statement->Operation(),
            std::move( expressionLeft ),
            std::move( expressionRight ),
            statement->TrueLabel(),
            statement->FalseLabel()
        )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    updateLastStatement(
        new CJumpStatement( statement->Target() )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    updateLastStatement(
        new CLabelStatement( statement->Label() )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    statement->Destination()->Accept( this );
    std::unique_ptr<const CExpression> destination = std::move( lastExpression );

    statement->Source()->Accept( this );
    std::unique_ptr<const CExpression> source = std::move( lastExpression );

    updateLastStatement(
        new CMoveStatement( std::move( destination ), std::move( source ) )
    );

    onNodeExit( nodeName );
}

void CDoubleCallEliminationVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    statement->LeftStatement()->Accept( this );
    std::unique_ptr<const CStatement> statementLeft = std::move( lastStatement );

    statement->RightStatement()->Accept( this );
    std::unique_ptr<const CStatement> statementRight = std::move( lastStatement );

    updateLastStatement(
        new CSeqStatement( std::move( statementLeft ), std::move( statementRight ) )
    );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CDoubleCallEliminationVisitor::Visit( const CExpressionList* list ) {
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

void CDoubleCallEliminationVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    CStatementList* newList = new CStatementList();

    const std::vector<std::unique_ptr<const CStatement>>& statements = list->Statements();
    for ( auto it = statements.begin(); it != statements.end(); ++it ) {
        ( *it )->Accept( this );
        newList->Add( std::move( lastStatement ) );
    }

    updateLastStatementList( newList );

    onNodeExit( nodeName );
}
