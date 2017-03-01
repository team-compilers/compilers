#include <IRT/nodes/Expression.h>

#include <IRT/nodes/Statement.h>
#include <cassert>

using namespace IRTree;

CExpression::~CExpression() {}

CConstExpression::CConstExpression( int _value ) : value( _value ) {}
CConstExpression::~CConstExpression() {}

std::unique_ptr<const CExpression> CConstExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CConstExpression( value )
    ) );
}

std::unique_ptr<const CExpression> CConstExpression::Canonize() const {
    // not implemented yet
    assert( false );
}

CNameExpression::CNameExpression( CLabel _label ) : label( _label ) {}
CNameExpression::~CNameExpression() {}

std::unique_ptr<const CExpression> CNameExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CNameExpression( label )
    ) );
}

std::unique_ptr<const CExpression> CNameExpression::Canonize() const {
    // not implemented yet
    assert( false );
}

CTempExpression::CTempExpression( const CTemp& _temporary ) : temporary( _temporary ) {}
CTempExpression::~CTempExpression() {}

std::unique_ptr<const CExpression> CTempExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CTempExpression( temporary )
    ) );
}

std::unique_ptr<const CExpression> CTempExpression::Canonize() const {
    // not implemented yet
    assert( false );
}

CBinaryExpression::CBinaryExpression( TOperatorType _operation, const CExpression* left, const CExpression* right )
    : operation( _operation ), leftOperand( left ), rightOperand( right ) {}
CBinaryExpression::CBinaryExpression( TOperatorType _operation, std::unique_ptr<const CExpression> left, std::unique_ptr<const CExpression> right )
    : operation( _operation ), leftOperand( std::move( left ) ), rightOperand( std::move( right ) ) {}
CBinaryExpression::~CBinaryExpression() {}

std::unique_ptr<const CExpression> CBinaryExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CBinaryExpression(
            operation,
            leftOperand->Clone(),
            rightOperand->Clone()
        )
    ) );
}

std::unique_ptr<const CExpression> CBinaryExpression::Canonize() const {
    // not implemented yet
    assert( false );
}

CMemExpression::CMemExpression( const CExpression* _address ) : address( _address ) {}
CMemExpression::CMemExpression( std::unique_ptr<const CExpression> _address ) : address( std::move( _address ) ) {}
CMemExpression::~CMemExpression() {}

std::unique_ptr<const CExpression> CMemExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CMemExpression( address->Clone() )
    ) );
}

std::unique_ptr<const CExpression> CMemExpression::Canonize() const {
    // not implemented yet
    assert( false );
}

CCallExpression::CCallExpression( const CExpression* func, const CExpressionList* args )
    : function( func ), arguments( args ) {}
CCallExpression::CCallExpression( std::unique_ptr<const CExpression> func, std::unique_ptr<const CExpressionList> args )
    : function( std::move( func ) ), arguments( std::move( args ) ) {}
CCallExpression::~CCallExpression() {}

std::unique_ptr<const CExpression> CCallExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CCallExpression(
            function->Clone(),
            arguments->Clone()
        )
    ) );
}

std::unique_ptr<const CExpression> CCallExpression::Canonize() const {
    // not implemented yet
    assert( false );
}

CEseqExpression::CEseqExpression( const CStatement* _statement, const CExpression* _expression )
    : statement( _statement ), expression( _expression ) {}
CEseqExpression::CEseqExpression( std::unique_ptr<const CStatement> _statement, std::unique_ptr<const CExpression> _expression )
    : statement( std::move( _statement ) ), expression( std::move( _expression ) ) {}
CEseqExpression::~CEseqExpression() {}

std::unique_ptr<const CExpression> CEseqExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CEseqExpression(
            statement->Clone(),
            expression->Clone()
        )
    ) );
}


std::unique_ptr<const CExpression> CEseqExpression::Canonize() const {
    std::unique_ptr<const CStatement> statementCanonized = statement->Canonize();
    std::unique_ptr<const CExpression> expressionCanonized = expression->Canonize();
    // if ( dynamic_cast<const CEseqExpression*>( expressionCanonized ) ) {}
}
