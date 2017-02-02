// Author: Alexey Zhuravlev
// Description: Interface for visitor target. All targets for visitor must be inherited from it

#pragma once

#include <AST/visitors/Visitor.h>

namespace ASTree {

class IVisitorTarget {
public:
    virtual ~IVisitorTarget() {}
    virtual void Accept( IVisitor* visitor ) const = 0;
};

struct CLocation {
    CLocation( int _firstLine, int _lastLine, int _firstColumn, int _lastColumn )
        : firstLine( _firstLine ), lastLine( _lastLine ),
          firstColumn( _firstColumn ), lastColumn( _lastColumn ) {}
    int firstLine;
    int lastLine;
    int firstColumn;
    int lastColumn;
};

class CVisitorTarget : public IVisitorTarget {
public:
    CVisitorTarget( const CLocation& _location ) : location( _location ) {}
    virtual ~CVisitorTarget() {}
    virtual const CLocation& Location() const { return location; }
private:
    CLocation location;
};

}
