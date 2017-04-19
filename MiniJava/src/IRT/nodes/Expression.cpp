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

CNameExpression::CNameExpression( CLabel _label ) : label( _label ) {}
CNameExpression::~CNameExpression() {}

std::unique_ptr<const CExpression> CNameExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CNameExpression( label )
    ) );
}

CTempExpression::CTempExpression( const CTemp& _temporary ) : temporary( _temporary ) {}
CTempExpression::~CTempExpression() {}

std::unique_ptr<const CExpression> CTempExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CTempExpression( temporary )
    ) );
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

CMemExpression::CMemExpression( const CExpression* _address ) : address( _address ) {}
CMemExpression::CMemExpression( std::unique_ptr<const CExpression> _address ) : address( std::move( _address ) ) {}
CMemExpression::~CMemExpression() {}

std::unique_ptr<const CExpression> CMemExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CMemExpression( address->Clone() )
    ) );
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
