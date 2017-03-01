#include <IRT/nodes/Statement.h>

#include <IRT/nodes/Expression.h>
#include <cassert>

using namespace IRTree;

CStatement::~CStatement() {}

CMoveStatement::CMoveStatement( const CExpression* _destination, const CExpression* _source )
    : destination( _destination ), source( _source ) {}
CMoveStatement::CMoveStatement( std::unique_ptr<const CExpression> _destination, std::unique_ptr<const CExpression> _source )
    : destination( std::move( _destination ) ), source( std::move( _source ) ) {}
CMoveStatement::~CMoveStatement() {}

std::unique_ptr<const CStatement> CMoveStatement::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CStatement> CMoveStatement::Canonize() {
    // not implemented yet
    assert( false );
}

CExpStatement::CExpStatement( const CExpression* _expression ) : expression( _expression ) {}
CExpStatement::CExpStatement( std::unique_ptr<const CExpression> _expression )
    : expression( std::move( _expression ) ) {}
CExpStatement::~CExpStatement() {}

std::unique_ptr<const CStatement> CExpStatement::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CStatement> CExpStatement::Canonize() {
    // not implemented yet
    assert( false );
}

CJumpStatement::CJumpStatement( CLabel _target )
    : target( _target ) {}
CJumpStatement::~CJumpStatement() {}

std::unique_ptr<const CStatement> CJumpStatement::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CStatement> CJumpStatement::Canonize() {
    // not implemented yet
    assert( false );
}

CJumpConditionalStatement::CJumpConditionalStatement( TLogicOperatorType _operation,
    const CExpression* left, const CExpression* right,
    CLabel _labelTrue, CLabel _labelFalse )
        : leftOperand( left ), rightOperand( right ),
        labelTrue( _labelTrue ), labelFalse( _labelFalse ), operation( _operation ) {}
CJumpConditionalStatement::CJumpConditionalStatement( TLogicOperatorType _operation,
    std::unique_ptr<const CExpression> left, std::unique_ptr<const CExpression> right,
    CLabel _labelTrue, CLabel _labelFalse )
        : leftOperand( std::move( left ) ), rightOperand( std::move( right ) ),
        labelTrue( _labelTrue ), labelFalse( _labelFalse ), operation( _operation ) {}
CJumpConditionalStatement::~CJumpConditionalStatement() {}

std::unique_ptr<const CStatement> CJumpConditionalStatement::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CStatement> CJumpConditionalStatement::Canonize() {
    // not implemented yet
    assert( false );
}

CSeqStatement::CSeqStatement( const CStatement* _left, const CStatement* _right )
    : leftStatement( _left ), rightStatement( _right ) {}
CSeqStatement::CSeqStatement( std::unique_ptr<const CStatement> _left, std::unique_ptr<const CStatement> _right )
    : leftStatement( std::move( _left ) ), rightStatement( std::move( _right ) ) {}
CSeqStatement::~CSeqStatement() {}

std::unique_ptr<const CStatement> CSeqStatement::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CStatement> CSeqStatement::Canonize() {
    // not implemented yet
    assert( false );
}

CLabelStatement::CLabelStatement( CLabel _label ) : label( _label ) {}
CLabelStatement::~CLabelStatement() {}

std::unique_ptr<const CStatement> CLabelStatement::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CStatement> CLabelStatement::Canonize() {
    // not implemented yet
    assert( false );
}
