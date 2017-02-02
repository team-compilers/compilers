#pragma once

#include <memory>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/Expression.h>

namespace ASTree {

class CTypeModifier : public CVisitorTarget {
public:
    CTypeModifier( const CLocation& _location ) : CVisitorTarget( _location ) {}
    virtual ~CTypeModifier() {}
};

class CIntTypeModifier : public CTypeModifier {
public:
    CIntTypeModifier( const CLocation& _location ) : CTypeModifier( _location ) {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CBooleanTypeModifier : public CTypeModifier {
public:
    CBooleanTypeModifier( const CLocation& _location ) : CTypeModifier( _location ) {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CIntArrayTypeModifier : public CTypeModifier {
public:
    CIntArrayTypeModifier( const CLocation& _location ) : CTypeModifier( _location ) {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CIdTypeModifier : public CTypeModifier {
public:
    CIdTypeModifier( const CIdExpression* _typeId, const CLocation& _location )
        : CTypeModifier( _location ), typeId( _typeId ) {}

    const CIdExpression* TypeId() const { return typeId.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> typeId;
};

}
