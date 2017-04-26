#include <Synthesis/nodes/VisitorTarget.h>

namespace Synthesis {

class CAddCommand : public IVisitorTarget {
public:
    CAddCommand() {}
    ~CAddCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

}
