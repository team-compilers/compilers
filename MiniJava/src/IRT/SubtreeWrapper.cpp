#include <IRT/SubtreeWrapper.h>
#include <IRT/Temporary.h>

using namespace IRTree;

std::unique_ptr<const CExpression> CExpressionWrapper::ToExpression() {
    return std::move( expression );
}

std::unique_ptr<const CStatement> CExpressionWrapper::ToStatement() {
    return std::move( std::unique_ptr<const CStatement>( new CExpStatement( std::move( expression ) ) ) );
}

std::unique_ptr<const CStatement> CExpressionWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) {
    return std::move( std::unique_ptr<const CStatement>(
        new CJumpConditionalStatement(
            TLogicOperatorType::LOT_NE,
            std::move( expression ),
            std::move( std::unique_ptr<const CExpression>( new CConstExpression( 0 ) ) ),
            labelTrue,
            labelFalse
        )
    ) );
}


std::unique_ptr<const CExpression> CStatementWrapper::ToExpression() {
    // such calls should never happen
    assert( false );
    // return std::move( std::unique_ptr<const CExpression>(
    //     new CEseqExpression( statement, new CConstExpression( 0 ) )
    // ) );
}

std::unique_ptr<const CStatement> CStatementWrapper::ToStatement() {
    return std::move( statement );
}

std::unique_ptr<const CStatement> CStatementWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) {
    // such calls should never happen
    assert( false );
}


std::unique_ptr<const CExpression> CConditionalWrapper::ToExpression() {
    CTempExpression* tempExpression = new CTempExpression( CTemp() );
    CLabel labelFalse;
    CLabel labelTrue;
    return std::move( std::unique_ptr<const CExpression> (
        new CEseqExpression(
            new CSeqStatement(
                new CMoveStatement( tempExpression, new CConstExpression( 1 ) ),
                new CSeqStatement(
                    std::move( ToConditional( labelTrue, labelFalse ) ),
                    std::move( std::unique_ptr<const CSeqStatement>(
                        new CSeqStatement(
                            new CLabelStatement( labelFalse ),
                            new CSeqStatement(
                                new CMoveStatement( tempExpression, new CConstExpression( 0 ) ),
                                new CLabelStatement( labelTrue )
                            )
                        )
                    ) )
                )
            ),
            tempExpression
        )
    ) );
}

std::unique_ptr<const CStatement> CConditionalWrapper::ToStatement() {
    /* TODO; */
}

std::unique_ptr<const CStatement> CRelativeConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) {
    return std::move( std::unique_ptr<const CStatement>(
        new CJumpConditionalStatement(
            operatorType,
            std::move( operandLeft ),
            std::move( operandRight ),
            labelTrue,
            labelFalse
        )
    ) );
}

std::unique_ptr<const CStatement> CAndConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) {
    CLabel labelMiddle;
    return std::move( std::unique_ptr<const CStatement>(
        new CSeqStatement(
            std::move( operandLeft->ToConditional( labelMiddle, labelFalse ) ),
            std::move( std::unique_ptr<const CSeqStatement>(
                new CSeqStatement(
                    std::move( std::unique_ptr<const CLabelStatement>( new CLabelStatement( labelMiddle ) ) ),
                    std::move( operandRight->ToConditional( labelTrue, labelFalse ) )
                )
            ) )
        )
    ) );
}

std::unique_ptr<const CStatement> COrConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) {
    CLabel labelMiddle;
    return std::move( std::unique_ptr<const CStatement>(
        new CSeqStatement(
            std::move( operandLeft->ToConditional( labelTrue, labelMiddle ) ),
            std::move( std::unique_ptr<const CSeqStatement>(
                new CSeqStatement(
                    std::move( std::unique_ptr<const CLabelStatement>( new CLabelStatement( labelMiddle ) ) ),
                    std::move( operandRight->ToConditional( labelTrue, labelFalse ) )
                )
            ) )
        )
    ) );
}

std::unique_ptr<const CStatement> CNegateConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) {
    return std::move( wrapper->ToConditional( labelFalse, labelTrue ) ); // reversed order of arguments
}
