#pragma once

#include <cassert>
#include <IRT/Label.h>
#include <IRT/nodes/Expression.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

class ISubtreeWrapper {
public:
    virtual ~ISubtreeWrapper() {}
    virtual const CExpression* ToExpression() const = 0;
    virtual const CStatement* ToStatement() const = 0;
    virtual const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const = 0;
};

class CExpressionConverter : public ISubtreeWrapper {
public:
    CExpressionConverter( const CExpression* _expression ) : expression( _expression ) {}
    const CExpression* ToExpression() const { return expression; }
    const CStatement* ToStatement() const { return new CExpStatement( expression ); }
    const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const { /* TODO */ }
private:
    const CExpression* expression;
}

class CStatementConverter : public ISubtreeWrapper {
public:
    CStatementConverter( const CStatement* _statement ) : statement( _statement ) {}
    const CExpression* ToExpression() const { assert( false ); }
    const CStatement* ToStatement() const { return statement; }
    const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const { assert( false ); }
private:
    const CStatement* statement;
}

class CConditionalConverter : public ISubtreeWrapper {
public:
    CConditionalConverter( const CStatement* _statement ) : statement( _statement ) {}
    const CExpression* ToExpression() const { /* TODO */ }
    const CStatement* ToStatement() const { /* TODO */ }
    const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const { return conditional; }
private:
    const CStatement* conditional;
}

}
