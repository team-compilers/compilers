#pragma once

namespace IRTree {

class IVisitorTarget {
public:
    virtual ~IVisitorTarget() {}
    virtual void Accept( IVisitor* visitor ) const = 0;
};

} // namespace IRTree