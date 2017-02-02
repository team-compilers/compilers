// Author: Alexey Zhuravlev
// Description: Expression Interface and it's implementations

#pragma once

#include <memory>
#include <string>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/ExpressionList.h>

namespace ASTree {

class CExpression : public CVisitorTarget {
public:
    CExpression( const CLocation& _location ) : CVisitorTarget( _location ) {}
    virtual ~CExpression() {}
};

//-----------------------------------------------------------------------------------------------//
//
enum class TOperatorType : char {
    OT_Plus,
    OT_Minus,
    OT_Times,
    OT_Div,
    OT_Mod,
    OT_LT,
    OT_And,
    OT_Or
};

class CBinaryExpression : public CExpression {
public:

    CBinaryExpression( TOperatorType _operation, const CExpression* left, const CExpression* right, const CLocation& _location )
        : CExpression( _location ),
          operation( _operation ),
          leftOperand( left ),
          rightOperand( right ) {}

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

// a[b]
class CBracketExpression : public CExpression {
public:

    CBracketExpression( const CExpression* _containerExpression, const CExpression* _indexExpression, const CLocation& _location )
        : CExpression( _location ),
          containerExpression( _containerExpression ),
          indexExpression( _indexExpression ) {}

    const CExpression* ContainerExpression() const { return containerExpression.get(); }
    const CExpression* IndexExpression() const { return indexExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:

    std::unique_ptr<const CExpression> containerExpression; //a
    std::unique_ptr<const CExpression> indexExpression; //b
};

//-----------------------------------------------------------------------------------------------//

class CNumberExpression : public CExpression {
public:

    CNumberExpression( int _value, const CLocation& _location )
        : CExpression( _location ), value( _value ) {}

    int Value() const { return value; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    int value;
};

//-----------------------------------------------------------------------------------------------//

class CLogicExpression : public CExpression {
public:

    CLogicExpression( bool _value, const CLocation& _location )
        : CExpression( _location ), value( _value ) {}

    bool Value() const { return value; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    bool value;
};

//----------------------------------------------------------------------------------------------//

// Expression for identities (variables)
class CIdExpression : public CExpression {
public:

    CIdExpression( const std::string& _name, const CLocation& _location )
        : CExpression( _location ), name( _name ) {}

    const std::string& Name() const { return name; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::string name;
};


//----------------------------------------------------------------------------------------------//

class CLengthExpression : public CExpression {
public:

    CLengthExpression( const CExpression* _lengthTarget, const CLocation& _location )
        : CExpression( _location ), lengthTarget( _lengthTarget ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* LengthTarget() const { return lengthTarget.get(); }

private:

    std::unique_ptr<const CExpression> lengthTarget;
};

//-----------------------------------------------------------------------------------------------//

class CMethodExpression : public CExpression {
public:

    CMethodExpression( const CExpression* _callerExpression,
            const CIdExpression* _methodId,
            const CExpressionList* _arguments, const CLocation& _location )
        : CExpression( _location ),
          callerExpression( _callerExpression ),
          methodId( _methodId ),
          arguments( _arguments ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* CallerExpression() const { return callerExpression.get(); }
    const CIdExpression* MethodId() const { return methodId.get(); }
    const CExpressionList* Arguments() const { return arguments.get(); }

private:

    std::unique_ptr<const CExpression> callerExpression;
    std::unique_ptr<const CIdExpression> methodId;
    std::unique_ptr<const CExpressionList> arguments;
};

//-----------------------------------------------------------------------------------------------//

class CThisExpression : public CExpression {
public:

    CThisExpression( const CLocation& _location )
        : CExpression( _location ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};


//-----------------------------------------------------------------------------------------------//

class CNewArrayExpression : public CExpression {
public:

    CNewArrayExpression( const CExpression* _lengthExpression, const CLocation& _location )
        : CExpression( _location ), lengthExpression( _lengthExpression ) {}

    const CExpression* LengthExpression() const { return lengthExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CExpression> lengthExpression;
};

//-----------------------------------------------------------------------------------------------//

class CNewIdExpression : public CExpression {
public:

    CNewIdExpression( const CIdExpression* _targetId, const CLocation& _location )
        : CExpression( _location ), targetId( _targetId ) {}

    const CIdExpression* TargetId() const { return targetId.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CIdExpression> targetId;
};

//-----------------------------------------------------------------------------------------------//

class CNegateExpression : public CExpression {
public:

    CNegateExpression( const CExpression* _targetExpression, const CLocation& _location )
        : CExpression( _location ), targetExpression( _targetExpression ) {}

    const CExpression* TargetExpression() const { return targetExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CExpression> targetExpression;
};

}
