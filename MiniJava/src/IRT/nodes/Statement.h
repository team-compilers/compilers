#pragma once

#include <memory>
#include <IRT/Label.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class IStatement : public IVisitorTarget {
public:
    virtual ~IStatement() = default;
};

class CStatement : public IStatement {
public:
    CStatement() = default;
    virtual ~CStatement() = default;
};

enum class TLogicOperatorType : char {
    LOT_EQ,
    LOT_NE,
    LOT_LT,
    LOT_GT,
    LOT_LE,
    LOT_GE,
    LOT_ULT,
    LOT_ULE,
    LOT_UGT,
    LOT_UGE
};

//-----------------------------------------------------------------------------------------------//

class CMoveStatement : public CStatement {
public:
    CMoveStatement( const CExpression* _destination, const CExpression* _source )
        : destination( _destination ), source( _source ) {}

    const CExpression* Destination() const { return destination.get(); }
    const CExpression* Source() const { return source.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> destination;
    std::unique_ptr<const CExpression> source;
};

//-----------------------------------------------------------------------------------------------//

class CExpStatement : public CStatement {
public:
    CExpStatement( const CExpression* _expression ) : expression( _expression ) {}

    const CExpression* Expression() const { return expression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> expression;
};

//-----------------------------------------------------------------------------------------------//

class CJumpStatement : public CStatement {
public:
    CJumpStatement( CLabel _target )
        : target( _target ) {}

    CLabel Target() const { return target; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    CLabel target;
};

//-----------------------------------------------------------------------------------------------//

class CJumpConditionalStatement : public CStatement {
public:
    CJumpConditionalStatement( TLogicOperatorType _operation,
        const CExpression* left, const CExpression* right,
        const CLabelStatement* _labelTrue, const CLabelStatement* _falseLabel )
            : leftOperand( left ), rightOperand( right ),
            labelTrue( _labelTrue ), labelFalse( _falseLabel ), operation( _operation ) {}

    const CExpression* LeftOperand() const { return leftOperand.get(); }
    const CExpression* RightOperand() const { return rightOperand.get(); }
    const CLabelStatement* TrueLabel() const { return labelTrue.get(); }
    const CLabelStatement* FalseLabel() const { return labelFalse.get(); }
    TLogicOperatorType Operation() const { return operation; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> leftOperand;
    std::unique_ptr<const CExpression> rightOperand;
    std::unique_ptr<const CLabelStatement> labelTrue;
    std::unique_ptr<const CLabelStatement> labelFalse;
    TLogicOperatorType operation;
};

//-----------------------------------------------------------------------------------------------//

class CSeqStatement : public CStatement {
public:
    CSeqStatement( const CStatement* _left, const CStatement* _right )
        : leftStatement( _left ), rightStatement( _right ) {}

    const CStatement* LeftStatement() const { return leftStatement.get(); }
    const CStatement* RightStatement() const { return rightStatement.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CStatement> leftStatement;
    std::unique_ptr<const CStatement> rightStatement;
};

//-----------------------------------------------------------------------------------------------//

class CLabelStatement : public CStatement {
public:
    CLabelStatement( CLabel _label ) : label( _label ) {}

    CLabel Label() const { return label; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    CLabel label;
};

} // namespace IRTree
