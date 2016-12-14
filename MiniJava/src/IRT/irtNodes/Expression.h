#include <irNodes/VisitorTarget.h>

namespace IRTree {

class IExpression : public IVisitorTarget {

};

//-----------------------------------------------------------------------------------------------//

class CConstExpression : public IExpression {
public:
    CConstExpression( int _value ) : value( _value ) {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    int Value() { return value; }
private:
    int value;
};

//-----------------------------------------------------------------------------------------------//



} // namespace IRTree
