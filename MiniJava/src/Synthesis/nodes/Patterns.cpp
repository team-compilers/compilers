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

const Synthesis::CExpression* CPattern::GetDynamicValue( const IRTVT* node ) {
    return std::get<1>(dynamic->find(node)->second).get();
}

template <typename Command>
void CPattern::ConsumeBinaryOperation( const IRTVT* node, IRTree::TOperatorType operation ) {
    auto root = GetTypedNode<CBinaryExpression>(node);
    if( !root.IsValid() || root->Operation() != operation ) {
        return;
    }
    
    int price = GetDynamicPrice(root->LeftOperand()) +
        GetDynamicPrice(root->RightOperand()) + 1;
    if(GetDynamicPrice(*root) > price) {
        (*dynamic)[*root] = std::make_tuple(price,
            std::unique_ptr<const CExpression>(new Command(
                GetDynamicValue(root->LeftOperand()),
                GetDynamicValue(root->RightOperand())
            )));
    }
}

void CAddPattern::Consume( const IRTVT* node ) {
    ConsumeBinaryOperation<CAddCommand>( node, IRTree::TOperatorType::OT_Plus );
}

void CSubPattern::Consume( const IRTVT* node ) {
    ConsumeBinaryOperation<CSubCommand>( node, IRTree::TOperatorType::OT_Minus );
}

void CMultPattern::Consume( const IRTVT* node ) {
    ConsumeBinaryOperation<CMultCommand>( node, IRTree::TOperatorType::OT_Times );
}

void CDivPattern::Consume( const IRTVT* node ) {
    ConsumeBinaryOperation<CDivCommand>( node, IRTree::TOperatorType::OT_Div );
}

void CSubConstPattern::Consume( const IRTVT* node ) {
}

void CAddConstLeftPattern::Consume( const IRTVT* node ) {
}

void CAddConstRightPattern::Consume( const IRTVT* node ) {
}

void CConstPattern::Consume( const IRTVT* node ) {
}

void CLoadMemoryPattern::Consume( const IRTVT* node ) {
}

void CLoadMemoryLeftOffsetPattern::Consume( const IRTVT* node ) {
}

void CLoadMemoryRightOffsetPattern::Consume( const IRTVT* node ) {
}

void CStoreMemoryPattern::Consume( const IRTVT* node ) {
}

void CStoreConstMemoryPattern::Consume( const IRTVT* node ) {
}

void CStoreMemoryRightOffsetPattern::Consume( const IRTVT* node ) {
}

void CStoreMemoryLeftOffsetPattern::Consume( const IRTVT* node ) {
}

void CMoveMemoryPattern::Consume( const IRTVT* node ) {
}