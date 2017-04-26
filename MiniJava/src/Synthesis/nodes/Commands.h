#include <Synthesis/nodes/VisitorTarget.h>

namespace Synthesis {

class CAddCommand : public CExpression {
public:
    CAddCommand(const CExpression* _leftOperand, const CExpression* _rightOperand) :
        leftOperand(_leftOperand), rightOperand(_rightOperand) {}
    ~CAddCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* leftOperand;
    const CExpression* rightOperand;
};

}
