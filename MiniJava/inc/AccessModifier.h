#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>

class IAccessModifier : public IVisitorTarget {
};

class CPublicAccessModifier : public IAccessModifier {
public:
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }
};

class CPrivateAccessModifier : public IAccessModifier {
public:
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }
};
