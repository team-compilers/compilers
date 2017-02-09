// Author: Alexey Zhuravlev
// Description: Statement Interface and it's implementations

#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/Expression.h>

namespace ASTree {

class CStatement : public CVisitorTarget {
public:
    CStatement( const CLocation& _location ) : CVisitorTarget( _location ) {}
    virtual ~CStatement() {}
};

class CStatementList;
//-----------------------------------------------------------------------------------------------//

class CAssignIdStatement : public CStatement {
public:

    CAssignIdStatement( const CIdExpression* _leftPart, const CExpression* _rightPart, const CLocation& _location )
        : CStatement( _location ),
          leftPart( _leftPart ),
          rightPart( _rightPart ) {}

    const CIdExpression* LeftPart() const { return leftPart.get(); }
    const CExpression* RightPart() const { return rightPart.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:

    std::unique_ptr<const CIdExpression> leftPart;
    std::unique_ptr<const CExpression> rightPart;
};

//-----------------------------------------------------------------------------------------------//

class CAssignIdWithIndexStatement : public CStatement {
public:

    CAssignIdWithIndexStatement( const CIdExpression* _leftPartId, const CExpression* _leftPartIndex,
        const CExpression* _rightPart, const CLocation& _location )
        : CStatement( _location ),
          leftPartId( _leftPartId ),
          leftPartIndex( _leftPartIndex ),
          rightPart( _rightPart ) {}

    const CIdExpression* LeftPartId() const { return leftPartId.get(); }
    const CExpression* LeftPartIndex() const { return leftPartIndex.get(); }
    const CExpression* RightPart() const { return rightPart.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:

    std::unique_ptr<const CIdExpression> leftPartId;
    std::unique_ptr<const CExpression> leftPartIndex;
    std::unique_ptr<const CExpression> rightPart;
};

//-----------------------------------------------------------------------------------------------//

class CPrintStatement : public CStatement {
public:
    CPrintStatement( const CExpression* _printTarget, const CLocation& _location )
        : CStatement( _location ), printTarget( _printTarget ) {}

    const CExpression* PrintTarget() const { return printTarget.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> printTarget;
};


//-----------------------------------------------------------------------------------------------//

class CConditionalStatement : public CStatement {
public:

    CConditionalStatement( const CExpression* _condition, const CStatement* _positiveTarget,
        const CStatement* _negativeTarget, const CLocation& _location )
        : CStatement( _location ),
          condition( _condition ),
          positiveTarget( _positiveTarget ),
          negativeTarget( _negativeTarget ) {}

    const CExpression* Condition() const { return condition.get(); }
    const CStatement* PositiveTarget() const { return positiveTarget.get(); }
    const CStatement* NegativeTarget() const { return negativeTarget.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CExpression> condition;
    std::unique_ptr<const CStatement> positiveTarget;
    std::unique_ptr<const CStatement> negativeTarget;
};

//-----------------------------------------------------------------------------------------------//

class CWhileLoopStatement : public CStatement {
public:

    CWhileLoopStatement( const CExpression* _condition, const CStatement* _body, const CLocation& _location )
        : CStatement( _location ),
        condition( _condition ),
        body( _body ) {}

    const CExpression* Condition() const { return condition.get(); }
    const CStatement* Body() const { return body.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CExpression> condition;
    std::unique_ptr<const CStatement> body;
};

//-----------------------------------------------------------------------------------------------//

class CBracesStatement : public CStatement {
public:

    CBracesStatement( const CStatementList* _list, const CLocation& _location )
        : CStatement( _location ), list( _list ) {}

    const CStatementList* List() const { return list.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CStatementList> list;
};

}
