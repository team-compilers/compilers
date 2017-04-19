#pragma once

#include <memory>

#include <IRT/Temporary.h>
#include <IRT/Label.h>

#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class CExpression : public IVisitorTarget {
public:
    virtual ~CExpression();
    virtual std::unique_ptr<const CExpression> Clone() const = 0;
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
    CConstExpression( int _value );
    ~CConstExpression();

    int Value() const { return value; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    int value;
};

//-----------------------------------------------------------------------------------------------//

class CNameExpression : public CExpression {
public:
    CNameExpression( CLabel _label );
    ~CNameExpression();

    const CLabel Label() const { return label; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    CLabel label;
};

//-----------------------------------------------------------------------------------------------//

class CTempExpression : public CExpression {
public:
    CTempExpression( const CTemp& _temporary );
    ~CTempExpression();

    CTemp Temporary() const { return temporary; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    CTemp temporary;
};

//-----------------------------------------------------------------------------------------------//

class CBinaryExpression : public CExpression {
public:
    CBinaryExpression( TOperatorType _operation, const CExpression* left, const CExpression* right );
    CBinaryExpression( TOperatorType _operation, std::unique_ptr<const CExpression> left, std::unique_ptr<const CExpression> right );
    ~CBinaryExpression();

    TOperatorType Operation() const { return operation; }
    const CExpression* LeftOperand() const { return leftOperand.get(); }
    const CExpression* RightOperand() const { return rightOperand.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    std::unique_ptr<const CExpression> leftOperand;
    std::unique_ptr<const CExpression> rightOperand;
    TOperatorType operation;
};

//-----------------------------------------------------------------------------------------------//

class CMemExpression : public CExpression {
public:
    CMemExpression( const CExpression* _address );
    CMemExpression( std::unique_ptr<const CExpression> _address );
    ~CMemExpression();

    const CExpression* Address() const { return address.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    std::unique_ptr<const CExpression> address;
};

//-----------------------------------------------------------------------------------------------//

class CCallExpression : public CExpression {
public:
    CCallExpression( const CExpression* func, const CExpressionList* args );
    CCallExpression( std::unique_ptr<const CExpression> func, std::unique_ptr<const CExpressionList> args );
    ~CCallExpression();

    const CExpression* Function() const { return function.get(); }
    const CExpressionList* Arguments() const { return arguments.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    std::unique_ptr<const CExpression> function;
    std::unique_ptr<const CExpressionList> arguments;
};

//-----------------------------------------------------------------------------------------------//
class CEseqExpression : public CExpression {
public:
    CEseqExpression( const CStatement* _statement, const CExpression* _expression );
    CEseqExpression( std::unique_ptr<const CStatement> _statement, std::unique_ptr<const CExpression> _expression );
    ~CEseqExpression();

    const CStatement* Statement() const { return statement.get(); }
    const CExpression* Expression() const { return expression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CExpression> Clone() const override;

private:
    std::unique_ptr<const CStatement> statement;
    std::unique_ptr<const CExpression> expression;
};

} // namespace IRTree
