// Author: Alexey Zhuravlev
// Description: Interface for visitor target. All targets for visitor must be inherited from it

#ifndef VISITOR_TARGET
#define VISITOR_TARGET

#include <Visitor.h>

class IVisitorTarget {
public:
    virtual void Accept( IVisitor* visitor ) = 0;
};

#endif
