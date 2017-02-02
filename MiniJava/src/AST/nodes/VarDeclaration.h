#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/Expression.h>
#include <AST/nodes/TypeModifier.h>

namespace ASTree {

class CVarDeclaration : public CVisitorTarget {
public:

    CVarDeclaration( const CTypeModifier* _type, const CIdExpression* _id, const CLocation& _location )
        : CVisitorTarget( _location ), type( _type ), id( _id ) {}

    const CTypeModifier* Type() const { return type.get(); }
    const CIdExpression* Id() const { return id.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CTypeModifier> type;
    std::unique_ptr<const CIdExpression> id;
};

}
