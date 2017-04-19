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
    return std::move( std::unique_ptr<const CStatement>(
        new CMoveStatement(
            destination->Clone(),
            source->Clone()
        )
    ) );
}

CExpStatement::CExpStatement( const CExpression* _expression ) : expression( _expression ) {}
CExpStatement::CExpStatement( std::unique_ptr<const CExpression> _expression )
    : expression( std::move( _expression ) ) {}
CExpStatement::~CExpStatement() {}

std::unique_ptr<const CStatement> CExpStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CExpStatement( expression->Clone() )
    ) );
}

CJumpStatement::CJumpStatement( CLabel _target )
    : target( _target ) {}
CJumpStatement::~CJumpStatement() {}

std::unique_ptr<const CStatement> CJumpStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CJumpStatement( target )
    ) );
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
    return std::move( std::unique_ptr<const CStatement>(
        new CJumpConditionalStatement(
            operation,
            leftOperand->Clone(),
            rightOperand->Clone(),
            labelTrue,
            labelFalse
        )
    ) );
}

CSeqStatement::CSeqStatement( const CStatement* _left, const CStatement* _right )
    : leftStatement( _left ), rightStatement( _right ) {}
CSeqStatement::CSeqStatement( std::unique_ptr<const CStatement> _left, std::unique_ptr<const CStatement> _right )
    : leftStatement( std::move( _left ) ), rightStatement( std::move( _right ) ) {}
CSeqStatement::~CSeqStatement() {}

std::unique_ptr<const CStatement> CSeqStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CSeqStatement(
            leftStatement->Clone(),
            rightStatement->Clone()
        )
    ) );
}

CLabelStatement::CLabelStatement( CLabel _label ) : label( _label ) {}
CLabelStatement::~CLabelStatement() {}

std::unique_ptr<const CStatement> CLabelStatement::Clone() const {
    return std::move( std::unique_ptr<const CStatement>(
        new CLabelStatement( label )
    ) );
}

std::unique_ptr<const CStatement> CStatementList::Clone() const {
    CStatementList* newList = new CStatementList();
    for ( auto it = statements.begin(); it != statements.end(); ++it ) {
        newList->Add( std::move( ( *it )->Clone() ) );
    }
    return std::move( std::unique_ptr<const CStatementList>( newList ) );
}
