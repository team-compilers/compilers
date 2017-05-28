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

std::unique_ptr<const CStatement> CEseqEliminationVisitor::canonizeStatementSubtree( std::unique_ptr<const CStatement> statement ) const {
    CEseqEliminationVisitor visitor( verbose );
    statement->Accept( &visitor );
    return std::move( visitor.ResultStatementTree() );
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

    std::unique_ptr<const CExpression> resultExpression;

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

    expression->Function()->Accept( this );
    std::unique_ptr<const CExpression> functionCanonized = std::move( lastExpression );
    expression->Arguments()->Accept( this );
    std::unique_ptr<const CExpressionList> argumentsListCanonized = std::move( lastExpressionList );

    std::vector<std::unique_ptr<const CStatement>> newStatements;
    CExpressionList* tempExpressionList = new CExpressionList();

    const std::vector<std::unique_ptr<const CExpression>>& argumentsCanonized = argumentsListCanonized->Expressions();
    for ( auto it = argumentsCanonized.begin(); it != argumentsCanonized.end(); ++it ) {
        const CEseqExpression* argumentEseq = castToEseqExpression( ( *it ).get() );
        if ( argumentEseq ) {
            newStatements.push_back( std::move( argumentEseq->Statement()->Clone() ) );
        }

        CTemp temp;
        tempExpressionList->Add( new CTempExpression( temp ) );

        std::unique_ptr<const CExpression> moveSourceExpression;
        if ( argumentEseq ) {
            moveSourceExpression = std::move( argumentEseq->Expression()->Clone() );
        } else {
            moveSourceExpression = std::move( ( *it )->Clone() );
        }
        std::unique_ptr<const CStatement> moveStatement = std::move( std::unique_ptr<const CStatement>(
            new CMoveStatement(
                std::move( std::unique_ptr<const CExpression>(
                    new CTempExpression( temp )
                ) ),
                std::move( moveSourceExpression )
            )
        ) );
        newStatements.push_back( std::move( moveStatement ) );
    }

    std::unique_ptr<const CExpression> resultExpression;
    if ( !newStatements.empty() ) {
        std::unique_ptr<const CStatement> suffixStatement = std::move( newStatements.back() );
        newStatements.pop_back();
        for ( auto it = newStatements.begin(); it != newStatements.end(); ++it ) {
            suffixStatement = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    std::move( *it ),
                    std::move( suffixStatement )
                )
            ) );
        }

        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CEseqExpression(
                std::move( suffixStatement ),
                std::move( std::unique_ptr<const CExpression>(
                    new CCallExpression(
                        std::move( functionCanonized ),
                        std::move( std::unique_ptr<const CExpressionList>(
                            tempExpressionList
                        ) )
                    )
                ) )
            )
        ) );
    } else {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CCallExpression(
                std::move( functionCanonized ),
                std::move( argumentsListCanonized )
            )
        ) );
    }

    updateLastExpression( std::move( resultExpression ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );

    expression->Statement()->Accept( this );
    std::unique_ptr<const CStatement> statementCanonized = std::move( lastStatement );
    expression->Expression()->Accept( this );
    std::unique_ptr<const CExpression> expressionCanonized = std::move( lastExpression );

    const CEseqExpression* eseqExpression = castToEseqExpression( expressionCanonized.get() );
    std::unique_ptr<const CExpression> resultExpression;
    if ( eseqExpression ) {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CEseqExpression(
                std::move( std::unique_ptr<const CStatement>(
                    new CSeqStatement(
                        std::move( statementCanonized ),
                        std::move( eseqExpression->Statement()->Clone() )
                    )
                ) ),
                std::move( eseqExpression->Expression()->Clone() )
            )
        ) );
    } else {
        resultExpression = std::move( std::unique_ptr<const CExpression>(
            new CEseqExpression(
                std::move( statementCanonized ),
                std::move( expressionCanonized )
            )
        ) );
    }

    updateLastExpression( std::move( resultExpression ) );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CEseqEliminationVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );

    statement->Expression()->Accept( this );
    std::unique_ptr<const CExpression> expressionCanonized = std::move( lastExpression );

    const CEseqExpression* eseqExpression = castToEseqExpression( expressionCanonized.get() );
    std::unique_ptr<const CStatement> resultStatement;
    if ( eseqExpression ) {
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CSeqStatement(
                std::move( eseqExpression->Statement()->Clone() ),
                std::move( std::unique_ptr<const CStatement>(
                    new CExpStatement(
                        std::move( eseqExpression->Expression()->Clone() )
                    )
                ) )
            )
        ) );
    } else {
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CExpStatement(
                std::move( expressionCanonized )
            )
        ) );
    }

    updateLastStatement( std::move( resultStatement ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );

    statement->LeftOperand()->Accept( this );
    std::unique_ptr<const CExpression> leftOperandCanonized = std::move( lastExpression );
    statement->RightOperand()->Accept( this );
    std::unique_ptr<const CExpression> rightOperandCanonized = std::move( lastExpression );

    const CEseqExpression* leftOperandEseq = castToEseqExpression( leftOperandCanonized.get() );
    const CEseqExpression* rightOperandEseq = castToEseqExpression( rightOperandCanonized.get() );

    std::unique_ptr<const CStatement> resultStatement;

    if ( leftOperandEseq ) {
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CJumpConditionalStatement(
                statement->Operation(),
                std::move( leftOperandEseq->Expression()->Clone() ),
                std::move( rightOperandCanonized ),
                statement->TrueLabel(),
                statement->FalseLabel()
            )
        ) );
        if ( rightOperandEseq ) {
            resultStatement = std::move( canonizeStatementSubtree( std::move( resultStatement ) ) );
        }

        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CSeqStatement(
                std::move( leftOperandEseq->Statement()->Clone() ),
                std::move( resultStatement )
            )
        ) );
    } else if ( rightOperandEseq ) {
        if ( areCommuting( rightOperandEseq->Statement(), leftOperandCanonized.get() ) ) {
            resultStatement = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    std::move( rightOperandEseq->Statement()->Clone() ),
                    std::move( std::unique_ptr<const CStatement>(
                        new CJumpConditionalStatement(
                            statement->Operation(),
                            std::move( leftOperandCanonized ),
                            std::move( rightOperandEseq->Expression()->Clone() ),
                            statement->TrueLabel(),
                            statement->FalseLabel()
                        )
                    ) )
                )
            ) );
        } else {
            CTemp temp;
            resultStatement = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    new CMoveStatement(
                        std::move( std::unique_ptr<const CExpression>(
                            new CTempExpression( temp )
                        ) ),
                        std::move( leftOperandCanonized )
                    ),
                    new CSeqStatement(
                        std::move( rightOperandEseq->Statement()->Clone() ),
                        std::move( std::unique_ptr<const CStatement>(
                            new CJumpConditionalStatement(
                                statement->Operation(),
                                std::move( std::unique_ptr<const CExpression>(
                                    new CTempExpression( temp )
                                ) ),
                                std::move( rightOperandEseq->Expression()->Clone() ),
                                statement->TrueLabel(),
                                statement->FalseLabel()
                            )
                        ) )
                    )
                )
            ) );
        }
    } else {
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CJumpConditionalStatement(
                statement->Operation(),
                std::move( leftOperandCanonized ),
                std::move( rightOperandCanonized ),
                statement->TrueLabel(),
                statement->FalseLabel()
            )
        ) );
    }

    updateLastStatement( std::move( resultStatement ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );

    updateLastStatement( std::move( statement->Clone() ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );

    updateLastStatement( std::move( statement->Clone() ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );

    statement->Destination()->Accept( this );
    std::unique_ptr<const CExpression> destinationCanonized = std::move( lastExpression );
    statement->Source()->Accept( this );
    std::unique_ptr<const CExpression> sourceCanonized = std::move( lastExpression );

    const CEseqExpression* destinationEseq = castToEseqExpression( destinationCanonized.get() );
    const CEseqExpression* sourceEseq = castToEseqExpression( sourceCanonized.get() );

    std::unique_ptr<const CStatement> resultStatement;
    if ( destinationEseq ) {
        // left son is eseq
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CMoveStatement(
                std::move( destinationEseq->Expression()->Clone() ),
                std::move( sourceCanonized )
            )
        ) );
        if ( sourceEseq ) {
            // right son is eseq
            resultStatement = std::move( canonizeStatementSubtree( std::move( resultStatement ) ) );
        }
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CSeqStatement(
                std::move( destinationEseq->Statement()->Clone() ),
                std::move( resultStatement )
            )
        ) );
    } else if ( sourceEseq ) {
        // right son is eseq
        if ( areCommuting( sourceEseq->Statement(), destinationCanonized.get() ) ) {
            resultStatement = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    std::move( sourceEseq->Statement()->Clone() ),
                    std::move( std::unique_ptr<const CStatement>(
                        new CMoveStatement(
                            std::move( destinationCanonized ),
                            std::move( sourceEseq->Expression()->Clone() )
                        )
                    ) )
                )
            ) );
        } else {
            CTemp temp;
            resultStatement = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    new CMoveStatement(
                        std::move( std::unique_ptr<const CExpression>(
                            new CTempExpression( temp )
                        ) ),
                        std::move( destinationCanonized )
                    ),
                    new CSeqStatement(
                        std::move( sourceEseq->Statement()->Clone() ),
                        std::move( std::unique_ptr<const CStatement>(
                            new CMoveStatement(
                                std::move( std::unique_ptr<const CExpression>(
                                    new CTempExpression( temp )
                                ) ),
                                std::move( sourceEseq->Expression()->Clone() )
                            )
                        ) )
                    )
                )
            ) );
        }
    } else {
        resultStatement = std::move( std::unique_ptr<const CStatement>(
            new CMoveStatement(
                std::move( destinationCanonized ),
                std::move( sourceCanonized )
            )
        ) );
    }

    updateLastStatement( std::move( resultStatement ) );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );

    statement->LeftStatement()->Accept( this );
    std::unique_ptr<const CStatement> leftStatementCanonized = std::move( lastStatement );
    statement->RightStatement()->Accept( this );
    std::unique_ptr<const CStatement> rightStatementCanonized = std::move( lastStatement );

    updateLastStatement( new CSeqStatement(
        std::move( leftStatementCanonized ),
        std::move( rightStatementCanonized )
    ) );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CEseqEliminationVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_EXP );
    onNodeEnter( nodeName );

    CExpressionList* newList = new CExpressionList();
    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );
        newList->Add( std::move( lastExpression ) );
    }

    updateLastExpressionList( newList );

    onNodeExit( nodeName );
}

void CEseqEliminationVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}
