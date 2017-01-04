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

class CExpressionWrapper : public ISubtreeWrapper {
public:
    CExpressionWrapper( const CExpression* _expression ) : expression( _expression ) {}
    virtual const CExpression* ToExpression() const { return expression; }
    virtual const CStatement* ToStatement() const { return new CExpStatement( expression ); }
    virtual const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const { /* TODO */ }
private:
    const CExpression* expression;
}

class CStatementWrapper : public ISubtreeWrapper {
public:
    CStatementWrapper( const CStatement* _statement ) : statement( _statement ) {}
    virtual const CExpression* ToExpression() const { assert( false ); }
    virtual const CStatement* ToStatement() const { return statement; }
    virtual const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const { assert( false ); }
private:
    const CStatement* statement;
}

class CConditionalWrapper : public ISubtreeWrapper {
public:
    CConditionalWrapper( const CStatement* _statement ) : statement( _statement ) {}
    virtual const CExpression* ToExpression() const { /* return new CConstExpression( 0 or 1 ); */ }
    virtual const CStatement* ToStatement() const { /* return conditional; */ }
    virtual const CStatement* ToConditional( CLabel trueLabel, CLabel falseLabel ) const = 0;
private:
    const CStatement* conditional;
}

}
