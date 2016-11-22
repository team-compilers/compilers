#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>

class IAccessModifier : public CVisitorTarget {
public:
    IAccessModifier( const CLocation& _location ) : CVisitorTarget( _location ) {}
    virtual ~IAccessModifier() {}
};

class CPublicAccessModifier : public IAccessModifier {
public:
    CPublicAccessModifier( const CLocation& _location ) : IAccessModifier( _location ) {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CPrivateAccessModifier : public IAccessModifier {
public:
    CPrivateAccessModifier( const CLocation& _location ) : IAccessModifier( _location ) {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};
