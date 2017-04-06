#include <IRT/visitors/EseqEliminationVisitor.h>
#include <IRT/Temporary.h>

using namespace IRTree;

std::unique_ptr<const CStatement> CEseqEliminationVisitor::ResultTree() {
    return std::move( ResultStatementTree() );
}

std::unique_ptr<const CStatement> CEseqEliminationVisitor::ResultStatementTree() {
    return std::move( lastStatement );
}

std::unique_ptr<const CExpression> CEseqEliminationVisitor::ResultExpressionTree() {
    return std::move( lastExpression );
}

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

std::unique_ptr<const CExpression> CEseqEliminationVisitor::canonizeExpressionSubtree( std::unique_ptr<const CExpression> expression ) const {
    CEseqEliminationVisitor visitor( verbose );
    expression->Accept( &visitor );
    return std::move( visitor.ResultExpressionTree() );
}

bool CEseqEliminationVisitor::areCommuting( const CStatement* statement, const CExpression* expression ) {
    assert( statement != nullptr && expression != nullptr );
    const CExpStatement* expStatement = dynamic_cast<const CExpStatement*>( statement );
    bool isStatementEmpty = expStatement != 0 &&
        dynamic_cast<const CConstExpression*>( expStatement->Expression() ) != 0;
    return isStatementEmpty ||
        dynamic_cast<const CConstExpression*>( expression ) != 0 ||
        dynamic_cast<const CNameExpression*>( expression ) != 0;
}

const CEseqExpression* CEseqEliminationVisitor::castToEseqExpression( const CExpression* expression ) {
    return dynamic_cast<const CEseqExpression*>( expression );
}

/*__________ Expressions __________*/

void CEseqEliminationVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );

    updateLastExpression( std::move( expression->Clone() ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );

    updateLastExpression( std::move( expression->Clone() ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    updateLastExpression( std::move( expression->Clone() ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept( this );
    std::unique_ptr<const CExpression> leftOperandCanonized = std::move( lastExpression );
    expression->RightOperand()->Accept( this );
    std::unique_ptr<const CExpression> rightOperandCanonized = std::move( lastExpression );

    const CEseqExpression* leftOperandEseq = castToEseqExpression( leftOperandCanonized.get() );
    const CEseqExpression* rightOperandEseq = castToEseqExpression( rightOperandCanonized.get() );

    std::unique_ptr<const CExpression> resultExpression = nullptr;

    if ( leftOperandEseq ) {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CEseqExpression(
                std::move( leftOperandEseq->Statement()->Clone() ),
                std::move( std::unique_ptr<const CExpression>(
                    new CBinaryExpression(
                        expression->Operation(),
                        std::move( leftOperandEseq->Expression()->Clone() ),
                        std::move( rightOperandCanonized )
                    )
                ) )
            )
        ) );
        if ( rightOperandEseq ) {
            resultExpression = canonizeExpressionSubtree( std::move( resultExpression ) );
        }
    } else if ( rightOperandEseq ) {
        if ( areCommuting( rightOperandEseq->Statement(), leftOperandCanonized.get() ) ) {
            resultExpression = std::move( std::unique_ptr<const CExpression>(
                new CEseqExpression(
                    std::move( rightOperandEseq->Statement()->Clone() ),
                    std::move( std::unique_ptr<const CExpression>(
                        new CBinaryExpression(
                            expression->Operation(),
                            std::move( leftOperandCanonized ),
                            std::move( rightOperandEseq->Expression()->Clone() )
                        )
                    ) )
                )
            ) );
        } else {
            CTemp temp;
            resultExpression = std::move( std::unique_ptr<const CExpression>(
                new CEseqExpression(
                    new CMoveStatement(
                        std::move( std::unique_ptr<const CExpression>(
                            new CTempExpression( temp )
                        ) ),
                        std::move( leftOperandCanonized )
                    ),
                    new CEseqExpression(
                        std::move( rightOperandEseq->Statement()->Clone() ),
                        std::move( std::unique_ptr<const CExpression>(
                            new CBinaryExpression(
                                expression->Operation(),
                                std::move( std::unique_ptr<const CExpression>(
                                    new CTempExpression( temp )
                                ) ),
                                std::move( rightOperandEseq->Expression()->Clone() )
                            )
                        ) )
                    )
                )
            ) );
            resultExpression = std::move( canonizeExpressionSubtree( std::move( resultExpression ) ) );
        }
    } else {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CBinaryExpression(
                expression->Operation(),
                std::move( leftOperandCanonized ),
                std::move( rightOperandCanonized )
            )
        ) );
    }

    updateLastExpression( std::move( resultExpression ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );

    expression->Address()->Accept( this );
    std::unique_ptr<const CExpression> addressCanonized = std::move( lastExpression );

    const CEseqExpression* addressEseq = castToEseqExpression( addressCanonized.get() );

    std::unique_ptr<const CExpression> resultExpression = nullptr;
    if ( addressEseq ) {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CEseqExpression(
                std::move( addressEseq->Statement()->Clone() ),
                std::move( std::unique_ptr<const CExpression>(
                    new CMemExpression( std::move( addressEseq->Expression()->Clone() ) )
                ) )
            )
        ) );
    } else {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CMemExpression( std::move( addressCanonized ) )
        ) );
    }
    updateLastExpression( std::move( resultExpression ) );

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
