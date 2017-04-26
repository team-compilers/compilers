#include <Synthesis/nodes/Patterns.h>
#include <Synthesis/visitors/TypingVisitor.h>

using namespace Synthesis;

template<typename T>
struct ValidAndValue {
    ValidAndValue( bool _valid, const T& _value ) : valid( _valid ), value( _value ) {}
    bool valid;
    T value;
};

template <typename T>
ValidAndValue<const T*> GetTypedNode( const IVisitorTarget* node ) {
    CTypingVisitor<T> visitor;
    node->Accept( visitor );
    const T* pointer = visitor->GetNode();
    return ValidAndValue<const T*>(pointer == nullptr, pointer);
}

void CAddPattern::Consume( const IVisitorTarget* node ) {
    
}
