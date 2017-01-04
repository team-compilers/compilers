#pragma once

#include <memory>
#include <IRT/Label.h>
#include <IRT/nodes/LabelList.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class IStatement : public IVisitorTarget {
public:
    virtual ~IStatement() {}
};

class CStatement : public IStatement {
public:
    CStatement() {}
    virtual ~CStatement() {}
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

class CMoveStatement : CStatement {
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

class CExpStatement : CStatement {
public:
    CExpStatement( const CExpression* _expression ) : expression( _expression ) {}

    const CExpression* Expression() const { return expression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> expression;
};

//-----------------------------------------------------------------------------------------------//

class CJumpStatement : CStatement {
public:
    CJumpStatement( const CExpression* _expression, const CLabelList* _targets )
        : expression( _expression ), targets( _targets ) {}

    const CExpression* Expression() const { return expression.get(); }
    const CLabelList* Targets() const { return targets.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> expression;
    std::unique_ptr<const CLabelList> targets;
};

//-----------------------------------------------------------------------------------------------//

class CJumpConditionalStatement : CStatement {
public:
    CJumpConditionalStatement( TLogicOperatorType _operation,
        const CExpression* left, const CExpression* right,
        const CLabelStatement* _trueLabel, const CLabelStatement* _falseLabel )
            : leftOperand( left ), rightOperand( right ),
            trueLabel( _trueLabel ), falseLabel( _falseLabel ), operation( _operation ) {}

    const CExpression* LeftOperand() const { return leftOperand.get(); }
    const CExpression* RightOperand() const { return rightOperand.get(); }
    const CLabelStatement* TrueLabel() const { return trueLabel.get(); }
    const CLabelStatement* FalseLabel() const { return falseLabel.get(); }
    TLogicOperatorType Operation() const { return operation; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CExpression> leftOperand;
    std::unique_ptr<const CExpression> rightOperand;
    std::unique_ptr<const CLabelStatement> trueLabel;
    std::unique_ptr<const CLabelStatement> falseLabel;
    TLogicOperatorType operation;
};

//-----------------------------------------------------------------------------------------------//

class CSeqStatement : CStatement {
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

class CLabelStatement : CStatement {
public:
    CLabelStatement( CLabel _label ) : label( _label ) {}

    CLabel Label() const { return label; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    CLabel label;
};

} // namespace IRTree
