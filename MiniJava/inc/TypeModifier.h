
#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <Expression.h>
#include <memory>

class ITypeModifier : public IVisitorTarget {
public:
	virtual ~ITypeModifier() {}
};

class CIntTypeModifier : public ITypeModifier {
public:
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CBooleanTypeModifier : public ITypeModifier {
public:
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CIntArrayTypeModifier : public ITypeModifier {
public:
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CIdTypeModifier : public ITypeModifier {
public:

    CIdTypeModifier( const CIdExpression* _typeId ) 
        : typeId( _typeId ) {}

    const CIdExpression* TypeId() const { return typeId.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> typeId;
};
