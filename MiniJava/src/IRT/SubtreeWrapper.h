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

class CRelativeConditionalWrapper : public CConditionalWrapper {
public:
    CRelativeConditionalWrapper( TLogicOperatorType _operatorType, const CExpression* _operandLeft, const CExpression* _operandRight )
        : operatorType( _operatorType ), operandLeft( _operandLeft ), operandRight( _operandRight ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    TLogicOperatorType operatorType;
    const CExpression* operandLeft;
    const CExpression* operandRight;
};

class CAndConditionalWrapper : public CConditionalWrapper {
public:
    CAndConditionalWrapper( const ISubtreeWrapper* _operandLeft, const ISubtreeWrapper* _operandRight )
        : operandLeft( _operandLeft ), operandRight( _operandRight ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const ISubtreeWrapper* operandLeft;
    const ISubtreeWrapper* operandRight;
};

class COrConditionalWrapper : public CConditionalWrapper {
public:
    COrConditionalWrapper( const ISubtreeWrapper* _operandLeft, const ISubtreeWrapper* _operandRight )
        : operandLeft( _operandLeft ), operandRight( _operandRight ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    const ISubtreeWrapper* operandLeft;
    const ISubtreeWrapper* operandRight;
};

class CNegateConditionalWrapper : public CConditionalWrapper {
public:
    CNegateConditionalWrapper( std::unique_ptr<const ISubtreeWrapper>&& _wrapper )
        : wrapper( std::move( _wrapper ) ) {}

    virtual const CStatement* ToConditional( CLabel labelTrue, CLabel labelFalse ) const override;
private:
    std::unique_ptr<const ISubtreeWrapper> wrapper;
};

}
