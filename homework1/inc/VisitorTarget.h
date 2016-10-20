// Author: Alexey Zhuravlev
// Description: Interface for visitor target. All targets for visitor must be inherited from it

#pragma once

#include <Visitor.h>

class IVisitorTarget {
public:
    virtual void Accept( IVisitor* visitor ) = 0;
};
