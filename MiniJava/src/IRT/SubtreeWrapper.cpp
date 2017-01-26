#include <IRT/SubtreeWrapper.h>
#include <IRT/Temporary.h>

using namespace IRTree;

const CExpression* CExpressionWrapper::ToExpression() const {
    return expression;
}

const CStatement* CExpressionWrapper::ToStatement() const {
    return new CExpStatement( expression );
}

const CStatement* CExpressionWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    // TODO: check it
    return new CJumpConditionalStatement(
        TLogicOperatorType::LOT_NE,
        expression,
        new CConstExpression( 0 ),
        new CLabelStatement( labelTrue ),
        new CLabelStatement( labelFalse )
    );
}


const CExpression* CStatementWrapper::ToExpression() const {
    // such calls should never happen
    assert( false );
    return new CEseqExpression( statement, new CConstExpression( 0 ) );
}

const CStatement* CStatementWrapper::ToStatement() const {
    return statement;
}

const CStatement* CStatementWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    // such calls should never happen
    assert( false );
}


const CExpression* CConditionalWrapper::ToExpression() const {
    CTempExpression* tempExpression = new CTempExpression( CTemp() );
    CLabel labelFalse;
    CLabel labelTrue;
    return new CEseqExpression( 
        new CSeqStatement(
            new CMoveStatement( tempExpression, new CConstExpression( 1 ) ),
            new CSeqStatement(
                ToConditional( labelTrue, labelFalse ),
                new CSeqStatement(
                    new CLabelStatement( labelFalse ),
                    new CSeqStatement(
                        new CMoveStatement( tempExpression, new CConstExpression( 0 ) ),
                        new CLabelStatement( labelTrue )
                    )
                )
            )
        ),
        tempExpression
    );
}

const CStatement* CConditionalWrapper::ToStatement() const {
    /* TODO; */
}

const CStatement* CRelativeConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    return new CJumpConditionalStatement(
        operatorType,
        operandLeft,
        operandRight,
        new CLabelStatement( labelTrue ),
        new CLabelStatement( labelFalse )
    );
}

const CStatement* CAndConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    CLabel labelMiddle;
    return new CSeqStatement(
        operandLeft->ToConditional( labelMiddle, labelFalse ),
        new CSeqStatement(
            new CLabelStatement( labelMiddle ),
            operandRight->ToConditional( labelTrue, labelFalse )
        )
    );
}

const CStatement* COrConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    CLabel labelMiddle;
    return new CSeqStatement(
        operandLeft->ToConditional( labelTrue, labelMiddle ),
        new CSeqStatement(
            new CLabelStatement( labelMiddle ),
            operandRight->ToConditional( labelTrue, labelFalse )
        )
    );
}

const CStatement* CNegateConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    return wrapper->ToConditional( labelFalse, labelTrue ); // reversed order of arguments
}
