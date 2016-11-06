// Author: Alexey Zhuravlev
// Description: Expression Interface and it's implementations

#pragma once

#include <memory>
#include <string>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <ExpressionList.h>

class IExpression : public IVisitorTarget {
public:
    virtual ~IExpression() {}
};

//-----------------------------------------------------------------------------------------------//
//
enum class TOperandType : char {
    OT_Plus,
    OT_Minus,
    OT_Times,
    OT_Div,
    OT_Mod,
    OT_LT,
    OT_And,
    OT_Or,

    OT_Count
};

class CBinaryExpression : public IExpression {
public:

    CBinaryExpression( TOperandType _operation, const IExpression* left, const IExpression* right )
        : operation( _operation ),
          leftOperand( left ),
          rightOperand( right ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    TOperandType Operation() const { return operation; }
    const IExpression* LeftOperand() const { return leftOperand.get(); }
    const IExpression* RightOperand() const { return rightOperand.get(); }

private:

    std::unique_ptr<const IExpression> leftOperand;
    std::unique_ptr<const IExpression> rightOperand;
    TOperandType operation;
};


//-----------------------------------------------------------------------------------------------//

// a[b]
class CBracketExpression : public IExpression {
public:

    CBracketExpression( const IExpression* _containerExpression, const IExpression* _indexExpression )
        : containerExpression( _containerExpression ),
          indexExpression( _indexExpression ) {}

    const IExpression* ContainerExpression() const { return containerExpression.get(); }
    const IExpression* IndexExpression() const { return indexExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:

    std::unique_ptr<const IExpression> containerExpression; //a
    std::unique_ptr<const IExpression> indexExpression; //b
};

//-----------------------------------------------------------------------------------------------//

class CNumberExpression : public IExpression {
public:

    CNumberExpression( int _value )
        : value( _value ) {}

    int Value() const { return value; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    int value;
};

//-----------------------------------------------------------------------------------------------//

class CLogicExpression : public IExpression {
public:

    CLogicExpression( bool _value ) 
        : value( _value ) {}

    bool Value() const { return value; }
    
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    bool value;
};

//----------------------------------------------------------------------------------------------//

// Expression for identities (variables)
class CIdExpression : public IExpression {
public:

    CIdExpression( const std::string& _name )
        : name( _name ) {}

    const std::string& Name() const { return name; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::string name;
};


//----------------------------------------------------------------------------------------------//

class CLengthExpression : public IExpression {
public:

    CLengthExpression( const IExpression* _lengthTarget )
        : lengthTarget( _lengthTarget ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
    
    const IExpression* LengthTarget() const { return lengthTarget.get(); }

private:

    std::unique_ptr<const IExpression> lengthTarget;
};

//-----------------------------------------------------------------------------------------------//

class CMethodExpression : public IExpression {
public:

    CMethodExpression( const IExpression* _callerExpression, 
            const CIdExpression* _methodId,
            const CExpressionList* _arguments ) 
        : callerExpression( _callerExpression ),
          methodId( _methodId ),
          arguments( _arguments ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const IExpression* CallerExpression() const { return callerExpression.get(); }
    const CIdExpression* MethodId() const { return methodId.get(); }
    const CExpressionList* Arguments() const { return arguments.get(); }

private:

    std::unique_ptr<const IExpression> callerExpression;
    std::unique_ptr<const CIdExpression> methodId;
    std::unique_ptr<const CExpressionList> arguments;
};

//-----------------------------------------------------------------------------------------------//

class CThisExpression : public IExpression {
public: 
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};


//-----------------------------------------------------------------------------------------------//

class CNewArrayExpression : public IExpression {
public:

    CNewArrayExpression( const IExpression* _lengthExpression )
        : lengthExpression( _lengthExpression ) {}

    const IExpression* LengthExpression() const { return lengthExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> lengthExpression;
};

//-----------------------------------------------------------------------------------------------//

class CNewIdExpression : public IExpression {
public:

    CNewIdExpression( const CIdExpression* _targetId )
        : targetId( _targetId ) {}

    const CIdExpression* TargetId() const { return targetId.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CIdExpression> targetId;
};

//-----------------------------------------------------------------------------------------------//

class CNegateExpression : public IExpression {
public:

    CNegateExpression( const IExpression* _targetExpression )
        : targetExpression( _targetExpression ) {}

    const IExpression* TargetExpression() const { return targetExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> targetExpression;
};
