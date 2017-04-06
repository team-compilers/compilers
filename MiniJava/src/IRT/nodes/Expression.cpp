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
//     return std::move( Clone() );
}

CNameExpression::CNameExpression( CLabel _label ) : label( _label ) {}
CNameExpression::~CNameExpression() {}

std::unique_ptr<const CExpression> CNameExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CNameExpression( label )
    ) );
}

std::unique_ptr<const CExpression> CNameExpression::Canonize() const {
//     return std::move( Clone() );
}

CTempExpression::CTempExpression( const CTemp& _temporary ) : temporary( _temporary ) {}
CTempExpression::~CTempExpression() {}

std::unique_ptr<const CExpression> CTempExpression::Clone() const {
    return std::move( std::unique_ptr<const CExpression>(
        new CTempExpression( temporary )
    ) );
}

std::unique_ptr<const CExpression> CTempExpression::Canonize() const {
//     return std::move( Clone() );
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
//     std::unique_ptr<const CExpression> expressionLeftCanon = leftOperand->Canonize();
//     std::unique_ptr<const CExpression> expressionRightCanon = rightOperand->Canonize();
//     std::unique_ptr<const CExpression> result;

//     const CEseqExpression* eseqExpressionLeftCanon = CastToEseqExpression( expressionLeftCanon.get() );
//     const CEseqExpression* eseqExpressionRightCanon = CastToEseqExpression( expressionRightCanon.get() );

//     if ( eseqExpressionLeftCanon ) {
//         result = std::move( std::unique_ptr<const CExpression>(
//             new CEseqExpression(
//                 std::move( eseqExpressionLeftCanon->Statement()->Clone() ),
//                 std::move( std::unique_ptr<const CExpression>(
//                     new CBinaryExpression(
//                         operation,
//                         std::move( eseqExpressionLeftCanon->Expression()->Clone() ),
//                         std::move( expressionRightCanon )
//                     )
//                 ) )
//             )
//         ) );
//     } else if ( eseqExpressionRightCanon ) {
//         if ( AreCommuting( eseqExpressionRightCanon->Statement(), expressionLeftCanon.get() ) ) {
//             result = std::move( std::unique_ptr<const CExpression>(
//                 new CEseqExpression(
//                     std::move( eseqExpressionRightCanon->Statement()->Clone() ),
//                     std::move( std::unique_ptr<const CExpression>(
//                         new CBinaryExpression(
//                             operation,
//                             std::move( expressionLeftCanon ),
//                             std::move( eseqExpressionRightCanon->Expression()->Clone() )
//                         )
//                     ) )
//                 )
//             ) );
//         } else {
//             CTemp temp;
//             result = std::move( std::unique_ptr<const CExpression>(
//                 new CEseqExpression(
//                     new CMoveStatement(
//                         std::move( std::unique_ptr<const CExpression>(
//                             new CTempExpression( temp )
//                         ) ),
//                         std::move( expressionLeftCanon )
//                     ),
//                     new CEseqExpression(
//                         std::move( eseqExpressionRightCanon->Statement()->Clone() ),
//                         std::move( std::unique_ptr<const CExpression>(
//                             new CBinaryExpression(
//                                 operation,
//                                 std::move( std::unique_ptr<const CExpression>(
//                                     new CTempExpression( temp )
//                                 ) ),
//                                 std::move( eseqExpressionRightCanon->Expression()->Clone() )
//                             )
//                         ) )
//                     )
//                 )
//             ) );
//         }
//     } else {
//         result = std::move( std::unique_ptr<const CExpression>(
//             new CBinaryExpression(
//                 operation,
//                 std::move( expressionLeftCanon ),
//                 std::move( expressionRightCanon )
//             )
//         ) );
//     }

//     return std::move( result );
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
//     std::unique_ptr<const CExpression> expressionCanon = address->Canonize();

//     std::unique_ptr<const CExpression> result;
//     const CEseqExpression* eseqExpressionCanon = CastToEseqExpression( expressionCanon.get() );
//     if ( eseqExpressionCanon ) {
//         result = std::move( std::unique_ptr<const CExpression>(
//             new CEseqExpression(
//                 std::move( eseqExpressionCanon->Statement()->Clone() ),
//                 std::move( std::unique_ptr<const CExpression>(
//                     new CMemExpression(
//                         eseqExpressionCanon->Expression()->Clone()
//                     )
//                 ) )
//             )
//         ) );
//     } else {
//         result = std::move( std::unique_ptr<const CExpression>(
//             new CMemExpression( std::move( expressionCanon ) )
//         ) );
//     }
//     return std::move( result );
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
//     std::unique_ptr<const CExpression> functionCanon = function->Canonize();
//     std::unique_ptr<const CExpressionList> argumentsCanonListPtr = arguments->Canonize();

//     std::unique_ptr<CExpressionList> expressionList( new CExpressionList() );
//     std::unique_ptr<CStatementList> statementList( new CStatementList() );

//     const std::vector<std::unique_ptr<const CExpression>>& argumentsCanonList = argumentsCanonListPtr->Expressions();
//     for ( auto it = std::next( argumentsCanonList.rbegin() ); it != argumentsCanonList.rend(); ++it ) {
//         std::unique_ptr<const CExpression> expressionSave;
//         const CEseqExpression* eseqExpressionCanon = CastToEseqExpression( it->get() );
//         if ( eseqExpressionCanon ) {
//             statementList->Add( std::move( eseqExpressionCanon->Statement()->Clone() ) );
//             expressionSave = std::move( eseqExpressionCanon->Expression()->Clone() );
//         } else {
//             expressionSave = std::move( ( *it )->Clone() );
//         }

//         CTemp temp;
//         statementList->Add(
//             std::move( std::unique_ptr<const CStatement>(
//                 new CMoveStatement(
//                     std::move( std::unique_ptr<const CExpression>(
//                         new CTempExpression( temp )
//                     ) ),
//                     std::move( expressionSave )
//                 )
//             ) )
//         );
//         expressionList->Add(
//             std::move( std::unique_ptr<const CExpression>(
//                 new CTempExpression( temp )
//             ) )
//         );
//     }

//     const std::vector<std::unique_ptr<const CStatement>>& statements = statementList->Statements();
//     std::unique_ptr<const CStatement> resultOnSuffix;
//     if ( !statements.empty() ) {// TODO: how is it possible?
//         resultOnSuffix = std::move( statements.back()->Clone() );
//         for ( auto it = std::next( statements.rbegin() ); it != statements.rend(); ++it ) {
//             resultOnSuffix = std::move( std::unique_ptr<const CStatement>(
//                 new CSeqStatement(
//                     std::move( ( *it )->Clone() ),
//                     std::move( resultOnSuffix )
//                 )
//             ) );
//         }
//     }

//     return std::move( std::unique_ptr<const CExpression>(
//         new CEseqExpression(
//             std::move( resultOnSuffix ),
//             std::move( std::unique_ptr<const CExpression>(
//                 new CCallExpression(
//                     std::move( functionCanon ),
//                     std::move( expressionList )
//                 )
//             ) )
//         )
//     ) );
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
//     std::unique_ptr<const CStatement> statementCanon = statement->Canonize();
//     std::unique_ptr<const CExpression> expressionCanon = expression->Canonize();

//     std::unique_ptr<const CExpression> result;
//     const CEseqExpression* eseqExpressionCanon = CastToEseqExpression( expressionCanon.get() );
//     if ( eseqExpressionCanon ) {
//         result = std::move( std::unique_ptr<const CExpression>(
//             new CEseqExpression(
//                 std::move( std::unique_ptr<const CStatement>(
//                     new CSeqStatement(
//                         std::move( statementCanon ),
//                         std::move( eseqExpressionCanon->Statement()->Clone() )
//                     )
//                 ) ),
//                 std::move( eseqExpressionCanon->Expression()->Clone() )
//             )
//         ) );
//     } else {
//         result = std::move( std::unique_ptr<const CExpression>(
//             new CEseqExpression(
//                 std::move( statementCanon ),
//                 std::move( expressionCanon )
//             )
//         ) );
//     }
//     return std::move( result );
}
