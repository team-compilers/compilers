#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <Expression.h>
#include <TypeModifier.h>

class CVarDeclaration : public IVisitorTarget {
public:

    CVarDeclaration( const ITypeModifier* _type, const CIdExpression* _id )
        : type( _type ),
          id( _id ) {}

    const ITypeModifier* Type() const { return type.get(); }
    const CIdExpression* Id() const { return id.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const ITypeModifier> type;
    std::unique_ptr<const CIdExpression> id;
};
