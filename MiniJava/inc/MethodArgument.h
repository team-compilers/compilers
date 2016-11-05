#pragma once

#include <Visitor.h>
#include <VisitorTarget.h>
#include <Expression.h>
#include <TypeModifier.h>
#include <vector>

class CMethodArgument : public IVisitorTarget {
public:

    CMethodArgument( const ITypeModifier* _argumentType, const CIdExpression* _argumentId )
        : argumentType( _argumentType ),
          argumentId( _argumentId ) {}

    const ITypeModifier* ArgumentType() const { return argumentType.get(); }
    const CIdExpression* ArgumentId() const { return argumentId.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:
    std::unique_ptr<const ITypeModifier> argumentType;
    std::unique_ptr<const CIdExpression> argumentId;
};
