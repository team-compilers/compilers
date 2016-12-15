#pragma once

#include <memory>

#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>
#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class IExpression : public IVisitorTarget {
public:
    virtual ~IExpression() {}
};

class CExpression : public IExpression {
public:
    virtual ~CExpression() {}
};

enum class TOperatorType : char {
    OT_Plus,
    OT_Minus,
    OT_Times,
    OT_Div,
    OT_And,
    OT_Or
};

//-----------------------------------------------------------------------------------------------//

class CConstExpression : public CExpression {
public:
    CConstExpression( int _value ) : value( _value ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    int Value() { return value; }
private:
    int value;
};

//-----------------------------------------------------------------------------------------------//

class CNameExpression : public CExpression {
public:
    CNameExpression( const CLabelStatement* _label ) : label( _label ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CLabelStatement* Label() const { return label.get(); }

private:
    std::unique_ptr<const CLabelStatement> label;
};

//-----------------------------------------------------------------------------------------------//

class CTempExpression : public CExpression {
public:
    CTempExpression() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

//-----------------------------------------------------------------------------------------------//

class CBinaryExpression : public CExpression {
public:
    CBinaryExpression( TOperatorType _operation, const CExpression* left, const CExpression* right )
        : operation( _operation ), leftOperand( left ), rightOperand( right ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    TOperatorType Operation() const { return operation; }
    const CExpression* LeftOperand() const { return leftOperand.get(); }
    const CExpression* RightOperand() const { return rightOperand.get(); }

private:
    std::unique_ptr<const CExpression> leftOperand;
    std::unique_ptr<const CExpression> rightOperand;
    TOperatorType operation;
};

//-----------------------------------------------------------------------------------------------//

class CMemExpression : public CExpression {
public:
    CMemExpression( const CExpression* _byteNumber ) : byteNumber( _byteNumber ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* ByteNumber() const { return byteNumber.get(); }

private:
    std::unique_ptr<const CExpression> byteNumber;
};

//-----------------------------------------------------------------------------------------------//

class CCallExpression : public CExpression {
public:
    CCallExpression( const CExpression* func, const CExpressionList* args )
        : function( func ), arguments( args ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* Function() const { return function.get(); }
    const CExpressionList* Arguments() const { return arguments.get(); }

private:
    std::unique_ptr<const CExpression> function;
    std::unique_ptr<const CExpressionList> arguments;
};

//-----------------------------------------------------------------------------------------------//

class CEseqExpression : public CExpression {
public:
    CEseqExpression( const CExpression* _expression, const CStatement* _statement )
        : expression( _expression ), statement( _statement ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* Expression() const { return expression.get(); }
    const CStatement* Statement() const { return statement.get(); }

private:
    std::unique_ptr<const CExpression> expression;
    std::unique_ptr<const CStatement> statement;
};

} // namespace IRTree
