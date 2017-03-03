#include <IRT/Canonization.h>
#include <IRT/nodes/Expression.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

bool AreCommuting( const CStatement* statement, const CExpression* expression ) {
    assert( statement != nullptr && expression != nullptr );
    const CExpStatement* expStatement = dynamic_cast<const CExpStatement*>( statement );
    bool isStatementEmpty = expStatement != 0 &&
        dynamic_cast<const CConstExpression*>( expStatement->Expression() ) != 0;
    return isStatementEmpty ||
        dynamic_cast<const CConstExpression*>( expression ) != 0 ||
        dynamic_cast<const CNameExpression*>( expression ) != 0;
}

const CEseqExpression* CastToEseqExpression( const CExpression* expression ) {
    return dynamic_cast<const CEseqExpression*>( expression );
}

}