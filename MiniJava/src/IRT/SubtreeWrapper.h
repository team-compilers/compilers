#pragma once

#include <cassert>
#include <IRT/Label.h>
#include <IRT/nodes/Expression.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

class ISubtreeWrapper {
public:
    virtual ~ISubtreeWrapper() = default;
    virtual const CExpression* ToExpression() const = 0;
    virtual const CStatement* ToStatement() const = 0;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const = 0;
};

class CExpressionWrapper : public ISubtreeWrapper {
public:
    CExpressionWrapper( const CExpression* _expression ) : expression( _expression ) {}
    virtual ~CExpressionWrapper() = default;

    virtual const CExpression* ToExpression() const override;
    virtual const CStatement* ToStatement() const override;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const CExpression* expression;
};

class CStatementWrapper : public ISubtreeWrapper {
public:
    CStatementWrapper( const CStatement* _statement ) : statement( _statement ) {}
    virtual ~CStatementWrapper() = default;

    virtual const CExpression* ToExpression() const override;
    virtual const CStatement* ToStatement() const override;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const CStatement* statement;
};

class CConditionalWrapper : public ISubtreeWrapper {
public:
    CConditionalWrapper( const CStatement* _conditional ) : conditional( _conditional ) {}
    virtual ~CConditionalWrapper() = default;

    virtual const CExpression* ToExpression() const override;
    virtual const CStatement* ToStatement() const override;
    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const = 0;
private:
    const CStatement* conditional;
};

class CRelativeConditionalWrapper : public CConditionalWrapper {
public:
    CRelativeConditionalWrapper( const CStatement* _conditional ) : CConditionalWrapper( _conditional ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    TLogicOperatorType operatorType;
    const CExpression* left;
    const CExpression* right;
};

}
