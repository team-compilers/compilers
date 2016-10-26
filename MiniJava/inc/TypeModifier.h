
#pragma once

#include <VisitorTarget.h>
#include <Expression.h>

class ITypeModifier : public IVisitorTarget {
};

class CIntTypeModifier : public ITypeModifier {
public:
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }
};

class CBooleanTypeModifier : public ITypeModifier {
public:
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }
};
class CIntArrayTypeModifier : public ITypeModifier {
public:
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }
};

class CIdTypeModifier : public ITypeModifier {
public:

    CIdTypeModifier( const CIdExpression* _typeId ) 
        : typeId( _typeId ) {}

    const CIdExpression* TypeId() const { return typeId; }

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> typeId;
};
