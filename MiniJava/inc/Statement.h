// Author: Alexey Zhuravlev
// Description: Statement Interface and it's implementations

#pragma once

#include <Visitor.h>
#include <VisitorTarget.h>
#include <Expression.h>
#include <memory>

class IStatement : public IVisitorTarget {
};

//-----------------------------------------------------------------------------------------------//

class CAssignIdStatement : public IStatement {
public:

    CAssignIdStatement( const CIdExpression* _leftPart, const IExpression* _rightPart) 
        : leftPart( _leftPart ),
          rightPart( _rightPart ) {}

    const CIdExpression* LeftPart() const { return leftPart; }
    const IExpression* RightPart() const { return rightPart; }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }
private:

    std::unique_ptr<const CIdExpression> leftPart;
    std::unique_ptr<const IExpression> rightPart;
};

//-----------------------------------------------------------------------------------------------//

class CAssignIdWithIndexStatement : public IStatement {
public:

    CAssignIdWithIndexStatement( const CIdExpression* _leftPartId, const IExpression* _leftPartIndex, const IExpression* _rightPart ) 
        : leftPartId( _leftPartId ),
          leftPartIndex( _leftPartIndex ),
          rightPart( _rightPart ) {}

    const CIdExpression* LeftPartId() const { return leftPartId; }
    const IExpression* LeftPartIndex() const { return leftPartIndex; }
    const IExpression* RightPart() const { return rightPart; }

private:

    std::unique_ptr<const CIdExpression> leftPartId;
    std::unique_ptr<const IExpression> leftPartIndex;
    std::unique_ptr<const IExpression> rightPart;
};

//-----------------------------------------------------------------------------------------------//

class CPrintStatement : public IStatement {
public:

    CPrintStatement( const IExpression* _printTarget ) 
        : printTarget( _printTarget ) {}

    const IExpression* PrintTarget() const { return printTarget; }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> printTarget;
};


//-----------------------------------------------------------------------------------------------//

class CConditionalStatement : public IStatement {
public:

    CConditionalStatement( const IExpression* _condition, const IExpression* _positiveTarget, const IExpression* _negativeTarget )
        : condition( _condition ),
          positiveTarget( _positiveTarget ),
          negativeTarget( _negativeTarget ) {}

    const IExpression* Condition() const { return condition; }
    const IExpression* PositiveTarget() const { return positveTarget; }
    const IExpression* NegativeTarget() const { return negativeTarget; }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> condition;
    std::unique_ptr<const IExpression> positiveTarget;
    std::unique_ptr<const IExpression> negativeTarget;
};

//-----------------------------------------------------------------------------------------------//

class CWhileLoopStatement : public IStatement {
public:

    CWhileLoopStatement( const IExpression* _condition, const IExpression* _body ) 
        : condition( _condition ),
        body( _body ) {}

    const IExpression* Condition() const { return condition; }
    const IExpression* Body() const { return body; }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const IExpression> condition;
    std::unique_ptr<const IExpression> body;
};

//-----------------------------------------------------------------------------------------------//

class CBracketsStatement : public IStatement {
   private:
};
