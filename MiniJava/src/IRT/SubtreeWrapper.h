#pragma once

#include <cassert>
#include <IRT/Label.h>
#include <IRT/nodes/Expression.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

// Base Subtree Wrappers

class ISubtreeWrapper {
public:
    virtual ~ISubtreeWrapper() = default;
    virtual const CExpression* ToExpression() const = 0;
    virtual const CStatement* ToStatement() const = 0;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const = 0;
};

class CExpressionWrapper : public ISubtreeWrapper {
public:
    explicit CExpressionWrapper( const CExpression* _expression ) : expression( _expression ) {}
    virtual ~CExpressionWrapper() = default;

    virtual const CExpression* ToExpression() const override;
    virtual const CStatement* ToStatement() const override;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const CExpression* expression;
};

class CStatementWrapper : public ISubtreeWrapper {
public:
    explicit CStatementWrapper( const CStatement* _statement ) : statement( _statement ) {}
    virtual ~CStatementWrapper() = default;

    virtual const CExpression* ToExpression() const override;
    virtual const CStatement* ToStatement() const override;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const CStatement* statement;
};

class CConditionalWrapper : public ISubtreeWrapper {
public:
    CConditionalWrapper() = default;
    virtual ~CConditionalWrapper() = default;

    virtual const CExpression* ToExpression() const override;
    virtual const CStatement* ToStatement() const override;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const = 0;
};

// Specific Subtree Wrappers

class CIfThenElseExpressionWrapper : public CExpressionWrapper {
public:
    CIfThenElseExpressionWrapper( const CExpression* _condition, const CExpression* _expressionTrue, const CExpression* _expressionFalse )
        : condition( _condition ), expressionTrue( _expressionTrue ), _expressionFalse( expressionFalse ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const CExpression* condition;
    const CExpression* expressionTrue;
    const CExpression* expressionFalse;

    CLabel labelThen;
    CLabel labelElse;
    CLabel labelJoin;
}

class CRelativeConditionalWrapper : public CConditionalWrapper {
public:
    CRelativeConditionalWrapper( TLogicOperatorType _operatorType, const CExpression* _left, const CExpression* _right )
        : operatorType( _operatorType ), left( _left ), right( _right ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    TLogicOperatorType operatorType;
    const CExpression* left;
    const CExpression* right;
};

}
