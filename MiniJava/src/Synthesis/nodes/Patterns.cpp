#include <Synthesis/nodes/Patterns.h>

#include <limits>

#include <Synthesis/visitors/TypingVisitor.h>
#include <Synthesis/nodes/Commands.h>
#include <IRT/nodes/Expression.h>

using namespace Synthesis;

template <typename T>
CPattern::ValidAndValue<const T*> CPattern::GetTypedNode( const IRTVT* node ) {
    CTypingVisitor<T> visitor;
    node->Accept( &visitor );
    const T* pointer = visitor.GetNode();
    return ValidAndValue<const T*>(pointer == nullptr, pointer);
}

int CPattern::GetDynamicPrice( const IRTVT* node ) {
    auto iterator = dynamic->find(node);
    if(iterator == dynamic->end()) {
        return std::numeric_limits<int>::max();
    } else {
        return std::get<0>(iterator->second);
    }
}

void CAddPattern::Consume( const IRTVT* node ) {
    auto root = GetTypedNode<CBinaryExpression>(node);
    if(root.IsValid()) {
        if(root->Operation() == IRTree::TOperatorType::OT_Plus) {
            int price = GetDynamicPrice(root->LeftOperand()) +
                GetDynamicPrice(root->RightOperand()) + 1;
            if(GetDynamicPrice(*root) > price) {
                auto addCommand = new CAddCommand(/* TODO */);
                (*dynamic)[*root] = std::tie(price, addCommand);
            }
        }
    }
}
