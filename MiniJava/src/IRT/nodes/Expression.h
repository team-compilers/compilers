#pragma once

#include <memory>

#include <IRT/Temporary.h>

#include <IRT/nodes/ExpressionList.h>
#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/Statement.h>
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
    OT_Mod,
    OT_And,
    OT_Or
};

//-----------------------------------------------------------------------------------------------//

class CConstExpression : public CExpression {
public:
    CConstExpression( int _value ) : value( _value ) {}

    int Value() { return value; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:
    int value;
};

//-----------------------------------------------------------------------------------------------//

class CNameExpression : public CExpression {
public:
    CNameExpression( const CLabelStatement* _label ) : label( _label ) {}

    const CLabelStatement* Label() const { return label.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CLabelStatement> label;
};

//-----------------------------------------------------------------------------------------------//

class CTempExpression : public CExpression {
public:
    CTempExpression( CTemp _temporary ) : temporary( _temporary ) {}

    CTemp Temporary() const { return temporary; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:
    CTemp temporary;
};

//-----------------------------------------------------------------------------------------------//

class CBinaryExpression : public CExpression {
public:
    CBinaryExpression( TOperatorType _operation, const CExpression* left, const CExpression* right )
        : operation( _operation ), leftOperand( left ), rightOperand( right ) {}

    TOperatorType Operation() const { return operation; }
    const CExpression* LeftOperand() const { return leftOperand.get(); }
    const CExpression* RightOperand() const { return rightOperand.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> leftOperand;
    std::unique_ptr<const CExpression> rightOperand;
    TOperatorType operation;
};

//-----------------------------------------------------------------------------------------------//

class CMemExpression : public CExpression {
public:
    CMemExpression( const CExpression* _byteNumber ) : byteNumber( _byteNumber ) {}

    const CExpression* ByteNumber() const { return byteNumber.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> byteNumber;
};

//-----------------------------------------------------------------------------------------------//

class CCallExpression : public CExpression {
public:
    CCallExpression( const CExpression* func, const CExpressionList* args )
        : function( func ), arguments( args ) {}

    const CExpression* Function() const { return function.get(); }
    const CExpressionList* Arguments() const { return arguments.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> function;
    std::unique_ptr<const CExpressionList> arguments;
};

//-----------------------------------------------------------------------------------------------//

class CEseqExpression : public CExpression {
public:
    CEseqExpression( const CStatement* _statement, const CExpression* _expression )
        : statement( _statement ), expression( _expression ) {}

    const CStatement* Statement() const { return statement.get(); }
    const CExpression* Expression() const { return expression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CStatement> statement;
    std::unique_ptr<const CExpression> expression;
};

} // namespace IRTree
