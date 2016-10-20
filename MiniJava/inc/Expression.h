// Author: Alexey Zhuravlev
// Description: Expression Interface and it's implementations

#pragma once

#include <Visitor.h>
#include <VisitorTarget.h>
#include <string>
#include <memory>

class IExpression : public IVisitorTarget {
};

//-----------------------------------------------------------------------------------------------//

enum class TBinaryOperandType : char {
    OT_Plus,
    OT_Minus,
    OT_Times,
    OT_Div,
    OT_Mod,
    OT_LT,
    OT_And,
    OT_Or

    OT_Count
};

class CBinaryExpression: public IExpression {
public:

    CBinaryExpression( TOperandType _operation, const IExpression* left, const IExpression* rightOperand )
        : operation( _operation ),
          leftOperand( left ),
          rightOperand( right ) {}

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

    TOperandType Operation() { return operation; }
    const IExpression* LeftOperand() { return leftOperand; }
    const IExpression* RightOperand() { return rightOperand; }

private:

    std::unique_ptr<const IExpression> leftOperand;
    std::unique_ptr<const IExpression> rightOperand;
    TOperandType operation;
};


//-----------------------------------------------------------------------------------------------//

// a[b]
class CBracketExpression: public IExpression {
public:

    CBracketExpression( const IExpression* _containterExpression, const IExpression* _indexExpression )
        : containterExpression( _containterExpression ),
          indexExpression( _indexExpression ) {}

    const IExpression* ContainerExpression() { return containerExpression; }
    const IExpression* IndexExpression() { return indexExpression; }

private:

    std::unique_ptr<const IExpression> containerExpression; //a
    std::unique_ptr<const IExpression> indexExpression; //b
};

//-----------------------------------------------------------------------------------------------//

class CNumberExpression : public IExpression {
public:

    CNumberExpression( int _value )
        : value(_value) {}

    int Value() { return value; }

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:

    int value;
};

//-----------------------------------------------------------------------------------------------//

class CLogicExpression : public IExpression {
public:

    CLogicExpressin( bool _value ) 
        : value( _value ) {}

    bool Value() { return value; }

private:

    bool value;
};

//----------------------------------------------------------------------------------------------//

// Expression for identities (variables)
class CIdExpression : public IExpression {
public:

    CIdExpression( const std::string& _name )
        : name( _name ) {}

    const std::string& Name() { return name; }

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:

    std::string name;
};


//----------------------------------------------------------------------------------------------//

class CLengthExpression : public IExpression {
public:

    CLengthExpression( const IExpression* _lengthTarget )
        : lengthTarget( _lengthTarget ) {}

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression*> lengthTarget;
};

//-----------------------------------------------------------------------------------------------//

class CListExpression;

class CMethodExpression : public IExpression {
public:

    CMethodExpression( const IExpression* _callerExpression, 
            const CIdExpression* _methodId,
            const CListExpression* _arguments ) 
        : callerExpression( _callerExpression ),
          methodId( _methodId ),
          arguments( _arguments ) {}

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> callerExpression;
    std::unique_ptr<const CIdExpression> methodId;
    std::unique_ptr<const CListExpression> arguments;
};

//-----------------------------------------------------------------------------------------------//

class CThisExpression : public IExpression {
public: 

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }
};


//-----------------------------------------------------------------------------------------------//

class CNewArrayExpression : public IExpression {
public:

    CNewArrayExpression( const IExpression* _lengthExpression )
        : lengthExpression( _lengthExpression ) {}

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> lengthExpression;
};

//-----------------------------------------------------------------------------------------------//

class CNewIdExpression : public IExpression {
public:

    CNewIdExpression( const CIdExpression* _targetId )
        : targetId( _targetId ) {}

private:

    std::unique_ptr<const CIdExpression> targetId;
};

//-----------------------------------------------------------------------------------------------//

class CNegateExpression : public IExpression {
public:

    CNegateExpression( const IExpression* _targetExpression )
        : targetExpression( _targetExpression ) {}

private:

    std::unique_ptr<const IExpression> targetExpression;
};
