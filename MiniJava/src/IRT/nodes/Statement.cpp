#include <IRT/nodes/Statement.h>
#include <IRT/nodes/Expression.h>
#include <IRT/Canonization.h>
#include <cassert>

using namespace IRTree;

CStatement::~CStatement() {}

CMoveStatement::CMoveStatement( const CExpression* _destination, const CExpression* _source )
    : destination( _destination ), source( _source ) {}
CMoveStatement::CMoveStatement( std::unique_ptr<const CExpression> _destination, std::unique_ptr<const CExpression> _source )
    : destination( std::move( _destination ) ), source( std::move( _source ) ) {}
CMoveStatement::~CMoveStatement() {}

std::unique_ptr<const CStatement> CMoveStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CMoveStatement(
            destination->Clone(),
            source->Clone()
        )
    ) );
}

std::unique_ptr<const CStatement> CMoveStatement::Canonize() const {
    std::unique_ptr<const CExpression> destinationCanon = destination->Canonize();
    std::unique_ptr<const CExpression> sourceCanon = source->Canonize();

    std::unique_ptr<const CStatement> result;
    const CEseqExpression* sourceCanonEseq = CastToEseqExpression( sourceCanon.get() );
    if ( sourceCanonEseq ) {
        result = std::move( std::unique_ptr<const CStatement>(
            new CSeqStatement(
                std::move( sourceCanonEseq->Statement()->Clone() ),
                std::move( std::unique_ptr<const CStatement>(
                    new CMoveStatement(
                        std::move( destinationCanon ),
                        std::move( sourceCanonEseq->Expression()->Clone() )
                    )
                ) )
            )
        ) );
    } else {
        result = std::move( std::unique_ptr<const CStatement>(
            new CMoveStatement(
                std::move( destinationCanon ),
                std::move( sourceCanon )
            )
        ) );
    }
    return std::move( result );
}

CExpStatement::CExpStatement( const CExpression* _expression ) : expression( _expression ) {}
CExpStatement::CExpStatement( std::unique_ptr<const CExpression> _expression )
    : expression( std::move( _expression ) ) {}
CExpStatement::~CExpStatement() {}

std::unique_ptr<const CStatement> CExpStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CExpStatement( expression->Clone() )
    ) );
}

std::unique_ptr<const CStatement> CExpStatement::Canonize() const {
    std::unique_ptr<const CExpression> expressionCanon = expression->Canonize();
    return std::move( std::unique_ptr<const CStatement>(
        new CExpStatement( std::move( expressionCanon ) )
    ) );
}

CJumpStatement::CJumpStatement( CLabel _target )
    : target( _target ) {}
CJumpStatement::~CJumpStatement() {}

std::unique_ptr<const CStatement> CJumpStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CJumpStatement( target )
    ) );
}

std::unique_ptr<const CStatement> CJumpStatement::Canonize() const {
    return std::move( Clone() );
}

CJumpConditionalStatement::CJumpConditionalStatement( TLogicOperatorType _operation,
    const CExpression* left, const CExpression* right,
    CLabel _labelTrue, CLabel _labelFalse )
        : leftOperand( left ), rightOperand( right ),
        labelTrue( _labelTrue ), labelFalse( _labelFalse ), operation( _operation ) {}
CJumpConditionalStatement::CJumpConditionalStatement( TLogicOperatorType _operation,
    std::unique_ptr<const CExpression> left, std::unique_ptr<const CExpression> right,
    CLabel _labelTrue, CLabel _labelFalse )
        : leftOperand( std::move( left ) ), rightOperand( std::move( right ) ),
        labelTrue( _labelTrue ), labelFalse( _labelFalse ), operation( _operation ) {}
CJumpConditionalStatement::~CJumpConditionalStatement() {}

std::unique_ptr<const CStatement> CJumpConditionalStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CJumpConditionalStatement(
            operation,
            leftOperand->Clone(),
            rightOperand->Clone(),
            labelTrue,
            labelFalse
        )
    ) );
}

std::unique_ptr<const CStatement> CJumpConditionalStatement::Canonize() const {
    std::unique_ptr<const CExpression> expressionLeftCanon = leftOperand->Canonize();
    std::unique_ptr<const CExpression> expressionRightCanon = rightOperand->Canonize();

    std::unique_ptr<const CStatement> result;
    const CEseqExpression* eseqExpressionLeftCanon = CastToEseqExpression( expressionLeftCanon.get() );
    const CEseqExpression* eseqExpressionRightCanon = CastToEseqExpression( expressionRightCanon.get() );
    if ( eseqExpressionLeftCanon ) {
        result = std::move( std::unique_ptr<const CStatement>(
            new CSeqStatement(
                std::move( eseqExpressionLeftCanon->Statement()->Clone() ),
                std::move( std::unique_ptr<const CStatement>(
                    new CJumpConditionalStatement(
                        operation,
                        std::move( eseqExpressionLeftCanon->Expression()->Clone() ),
                        std::move( expressionRightCanon ),
                        labelTrue,
                        labelFalse
                    )
                ) )
            )
        ) );
    } else if ( eseqExpressionRightCanon ) {
        if ( AreCommuting( eseqExpressionRightCanon->Statement(), expressionLeftCanon.get() ) ) {
            result = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    std::move( eseqExpressionRightCanon->Statement()->Clone() ),
                    std::move( std::unique_ptr<const CStatement>(
                        new CJumpConditionalStatement(
                            operation,
                            std::move( expressionLeftCanon ),
                            std::move( eseqExpressionRightCanon->Expression()->Clone() ),
                            labelTrue,
                            labelFalse
                        )
                    ) )
                )
            ) );
        } else {
            CTemp temp;
            result = std::move( std::unique_ptr<const CStatement>(
                new CSeqStatement(
                    new CMoveStatement(
                        std::move( std::unique_ptr<const CExpression>(
                            new CTempExpression( temp )
                        ) ),
                        std::move( expressionLeftCanon )
                    ),
                    new CSeqStatement(
                        std::move( eseqExpressionRightCanon->Statement()->Clone() ),
                        std::move( std::unique_ptr<const CStatement>(
                            new CJumpConditionalStatement(
                                operation,
                                std::move( std::unique_ptr<const CExpression>(
                                    new CTempExpression( temp )
                                ) ),
                                std::move( eseqExpressionRightCanon->Expression()->Clone() ),
                                labelTrue,
                                labelFalse
                            )
                        ) )
                    )
                )
            ) );
        }
    } else {
        result = std::move( std::unique_ptr<const CStatement>(
            new CJumpConditionalStatement(
                operation,
                std::move( expressionLeftCanon ),
                std::move( expressionRightCanon ),
                labelTrue,
                labelFalse
            )
        ) );
    }
    return std::move( result );
}

CSeqStatement::CSeqStatement( const CStatement* _left, const CStatement* _right )
    : leftStatement( _left ), rightStatement( _right ) {}
CSeqStatement::CSeqStatement( std::unique_ptr<const CStatement> _left, std::unique_ptr<const CStatement> _right )
    : leftStatement( std::move( _left ) ), rightStatement( std::move( _right ) ) {}
CSeqStatement::~CSeqStatement() {}

std::unique_ptr<const CStatement> CSeqStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CSeqStatement(
            leftStatement->Clone(),
            rightStatement->Clone()
        )
    ) );
}

std::unique_ptr<const CStatement> CSeqStatement::Canonize() const {
    std::unique_ptr<const CStatement> statementLeftCanon = leftStatement->Canonize();
    std::unique_ptr<const CStatement> statementRightCanon = rightStatement->Canonize();

    return std::move( std::unique_ptr<const CStatement>(
        new CSeqStatement(
            std::move( statementLeftCanon ),
            std::move( statementRightCanon )
        )
    ) );
}

CLabelStatement::CLabelStatement( CLabel _label ) : label( _label ) {}
CLabelStatement::~CLabelStatement() {}

std::unique_ptr<const CStatement> CLabelStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CLabelStatement( label )
    ) );
}

std::unique_ptr<const CStatement> CLabelStatement::Canonize() const {
    return std::move( Clone() );
}
