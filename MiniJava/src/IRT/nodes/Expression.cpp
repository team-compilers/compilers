#include <IRT/nodes/Expression.h>

#include <IRT/nodes/Statement.h>
#include <cassert>

using namespace IRTree;

CExpression::~CExpression() {}

CConstExpression::CConstExpression( int _value ) : value( _value ) {}
CConstExpression::~CConstExpression() {}

std::unique_ptr<const CExpression> CConstExpression::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CExpression> CConstExpression::Canonize() {
    // not implemented yet
    assert( false );
}

CNameExpression::CNameExpression( CLabel _label ) : label( _label ) {}
CNameExpression::~CNameExpression() {}

std::unique_ptr<const CExpression> CNameExpression::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CExpression> CNameExpression::Canonize() {
    // not implemented yet
    assert( false );
}

CTempExpression::CTempExpression( const CTemp& _temporary ) : temporary( _temporary ) {}
CTempExpression::~CTempExpression() {}

std::unique_ptr<const CExpression> CTempExpression::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CExpression> CTempExpression::Canonize() {
    // not implemented yet
    assert( false );
}

CBinaryExpression::CBinaryExpression( TOperatorType _operation, const CExpression* left, const CExpression* right )
    : operation( _operation ), leftOperand( left ), rightOperand( right ) {}
CBinaryExpression::CBinaryExpression( TOperatorType _operation, std::unique_ptr<const CExpression> left, std::unique_ptr<const CExpression> right )
    : operation( _operation ), leftOperand( std::move( left ) ), rightOperand( std::move( right ) ) {}
CBinaryExpression::~CBinaryExpression() {}

std::unique_ptr<const CExpression> CBinaryExpression::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CExpression> CBinaryExpression::Canonize() {
    // not implemented yet
    assert( false );
}

CMemExpression::CMemExpression( const CExpression* _address ) : address( _address ) {}
CMemExpression::CMemExpression( std::unique_ptr<const CExpression> _address ) : address( std::move( _address ) ) {}
CMemExpression::~CMemExpression() {}

std::unique_ptr<const CExpression> CMemExpression::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CExpression> CMemExpression::Canonize() {
    // not implemented yet
    assert( false );
}

CCallExpression::CCallExpression( const CExpression* func, const CExpressionList* args )
    : function( func ), arguments( args ) {}
CCallExpression::CCallExpression( std::unique_ptr<const CExpression> func, std::unique_ptr<const CExpressionList> args )
    : function( std::move( func ) ), arguments( std::move( args ) ) {}
CCallExpression::~CCallExpression() {}

std::unique_ptr<const CExpression> CCallExpression::Clone() const {
    // not implemented yet
    assert( false );
}

std::unique_ptr<const CExpression> CCallExpression::Canonize() {
    // not implemented yet
    assert( false );
}

CEseqExpression::CEseqExpression( const CStatement* _statement, const CExpression* _expression )
    : statement( _statement ), expression( _expression ) {}
CEseqExpression::CEseqExpression( std::unique_ptr<const CStatement> _statement, std::unique_ptr<const CExpression> _expression )
    : statement( std::move( _statement ) ), expression( std::move( _expression ) ) {}
CEseqExpression::~CEseqExpression() {}

std::unique_ptr<const CExpression> CEseqExpression::Clone() const {
    // not implemented yet
    assert( false );
}


std::unique_ptr<const CExpression> CEseqExpression::Canonize() {
    // not implemented yet
    assert( false );
}
