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

const CStatement* CIfThenElseExpressionWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    return new CSeqStatement(
        // LOL
    );
}

const CStatement* CRelativeConditionalWrapper::ToConditional( CLabel labelTrue, CLabel labelFalse ) const {
    return new CJumpConditionalStatement(
        operatorType,
        left,
        right,
        new CLabelStatement( labelTrue ),
        new CLabelStatement( labelFalse )
    );
}

