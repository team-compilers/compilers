#pragma once

#include <cassert>
#include <memory>

namespace IRTree {

class CExpression;
class CStatement;
class CEseqExpression;

bool AreCommuting( const CStatement* statement, const CExpression* expression );

const CEseqExpression* CastToEseqExpression( const CExpression* expression );

} // namespace IRTree
