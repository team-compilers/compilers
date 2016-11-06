#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>

class IAccessModifier : public IVisitorTarget {
public:
	virtual ~IAccessModifier() {}
};

class CPublicAccessModifier : public IAccessModifier {
public:
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CPrivateAccessModifier : public IAccessModifier {
public:
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};
