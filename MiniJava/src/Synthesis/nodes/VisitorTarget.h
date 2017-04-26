#pragma once

#include <Synthesis/visitors/Visitor.h>

namespace Synthesis {

class IVisitorTarget {
public:
    virtual ~IVisitorTarget() {}
    virtual void Accept( IVisitor* visitor ) const = 0;
};

class CExpression : public IVisitorTarget {};

}
