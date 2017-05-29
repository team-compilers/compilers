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
    return ValidAndValue<const T*>( pointer != nullptr, pointer );
}

int CPattern::GetDynamicPrice( const IRTVT* node ) {
    auto iterator = dynamic->find( node );
    if( iterator == dynamic->end() ) {
        return std::numeric_limits<int>::max();
    } else {
        return iterator->second.first;
    }
}

const Synthesis::CExpression* CPattern::GetDynamicValue( const IRTVT* node ) {
    return dynamic_cast<const CExpression*>( dynamic->find( node )->second.second.get() );
}

template <typename Command>
void CPattern::ConsumeBinaryOperation( const IRTVT* node, IRTree::TOperatorType operation ) {
    auto root = GetTypedNode<CBinaryExpression>( node );
    if( !root.IsValid() || root->Operation() != operation ) {
        return;
    }
    
    const int price = GetDynamicPrice( root->LeftOperand() ) +
        GetDynamicPrice( root->RightOperand() ) + 1;
    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new Command(
                GetDynamicValue( root->LeftOperand() ),
                GetDynamicValue( root->RightOperand() )
            ) ) );
    }
}

void CTempPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<IRTree::CTempExpression>( node );

    if( !root.IsValid() ) {
        return;
    }

    (*dynamic)[*root] = std::make_pair( 1,
        std::unique_ptr<const CExpression>( new Synthesis::CTempExpression(  
            root->Temporary().ToString()
        ) ) );
}

void CLabelPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<IRTree::CLabelStatement>( node );

    if( !root.IsValid() ) {
        return;
    }

    (*dynamic)[*root] = std::make_pair( 1,
        std::unique_ptr<const CCommand>( new Synthesis::CLabelDeclarationCommand(
            root->Label().ToString()
        ) ) );
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
    const auto root = GetTypedNode<CBinaryExpression>( node );
    if( !root.IsValid() || root->Operation() != IRTree::TOperatorType::OT_Minus ) {
        return;
    }

    const IRTree::CExpression* left = root->LeftOperand();
    const IRTree::CExpression* right = root->RightOperand();

    const auto rightConst = GetTypedNode<CConstExpression>( node );
    if( !rightConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( left ) + 1;
    
    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CSubConstCommand(
                GetDynamicValue( left ),
                rightConst->Value()
            ) ) );
    }
}

void CAddConstLeftPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CBinaryExpression>( node );
    if( !root.IsValid() || root->Operation() != IRTree::TOperatorType::OT_Plus ) {
        return;
    }

    const IRTree::CExpression* left = root->LeftOperand();
    const IRTree::CExpression* right = root->RightOperand();

    const auto leftConst = GetTypedNode<CConstExpression>( node );
    if( !leftConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( right ) + 1;
    
    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CAddConstCommand(
                GetDynamicValue( right ),
                leftConst->Value()
            ) ) );
    }
}

void CAddConstRightPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CBinaryExpression>( node );
    if( !root.IsValid() || root->Operation() != IRTree::TOperatorType::OT_Plus ) {
        return;
    }

    const IRTree::CExpression* left = root->LeftOperand();
    const IRTree::CExpression* right = root->RightOperand();

    const auto rightConst = GetTypedNode<CConstExpression>( node );
    if( !rightConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( left ) + 1;
    
    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CAddConstCommand(
                GetDynamicValue( left ),
                rightConst->Value()
            ) ) );
    }
}

void CConstPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CConstExpression>( node );
    if( !root.IsValid() ) {
        return;
    }

    const int price = 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CAddConstCommand(
                new CNullExpression(),
                root->Value()
            ) ) );
    }
}

void CLoadMemoryPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMemExpression>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* value = root->Address();
    const int price = GetDynamicPrice( value ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CLoadCommand(
                GetDynamicValue( value ),
                0
            ) ) );
    }
}

void CLoadConstMemoryPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMemExpression>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* value = root->Address();
    const auto constValue = GetTypedNode<CConstExpression>( value );
    if( !constValue.IsValid() ) {
        return;
    }
    const int price =  1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CLoadCommand(
                new CNullExpression(),
                constValue->Value()
            ) ) );
    }
}

void CLoadMemoryLeftOffsetPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMemExpression>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* child = root->Address();
    const auto expression = GetTypedNode<CBinaryExpression>( child );
    if( !expression.IsValid() || expression->Operation() != IRTree::TOperatorType::OT_Plus ) {
        return;
    }

    const IRTree::CExpression* left = expression->LeftOperand();
    const IRTree::CExpression* right = expression->RightOperand();

    const auto leftConst = GetTypedNode<CConstExpression>( node );
    if( !leftConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( right ) + 1;
    
    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CLoadCommand(
                GetDynamicValue( right ),
                leftConst->Value()
            ) ) );
    }
}

void CLoadMemoryRightOffsetPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMemExpression>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* child = root->Address();
    const auto expression = GetTypedNode<CBinaryExpression>( child );
    if( !expression.IsValid() || expression->Operation() != IRTree::TOperatorType::OT_Plus ) {
        return;
    }

    const IRTree::CExpression* left = expression->LeftOperand();
    const IRTree::CExpression* right = expression->RightOperand();

    const auto rightConst = GetTypedNode<CConstExpression>( node );
    if( !rightConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( left ) + 1;
    
    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CExpression>( new CLoadCommand(
                GetDynamicValue( left ),
                rightConst->Value()
            ) ) );
    }
}

void CStoreMemoryPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMoveStatement>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* dest = root->Destination();
    const IRTree::CExpression* source = root->Source();
    
    const auto destValue = GetTypedNode<CMemExpression>( dest );
    if( !destValue.IsValid() ){
        return;
    }

    const int price = GetDynamicPrice( destValue->Address() ) + GetDynamicPrice( source ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CStatement>( new CStoreCommand(
                GetDynamicValue( dest ),
                0,
                GetDynamicValue( source )
            ) ) );
    }
}

void CStoreConstMemoryPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMoveStatement>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* dest = root->Destination();
    const IRTree::CExpression* source = root->Source();
    
    const auto destValue = GetTypedNode<CMemExpression>( dest );
    if( !destValue.IsValid() ){
        return;
    }
    const IRTree::CExpression* destChild = destValue->Address();
    const auto constValue = GetTypedNode<CConstExpression>( destChild );
    if( !constValue.IsValid() ){
        return;
    }

    const int price = GetDynamicPrice( source ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CStatement>( new CStoreCommand(
                new CNullExpression(),
                constValue->Value(),
                GetDynamicValue( source )
            ) ) );
    }
}

void CStoreMemoryRightOffsetPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMoveStatement>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* dest = root->Destination();
    const IRTree::CExpression* source = root->Source();

    const auto memValue = GetTypedNode<CMemExpression>( dest );
    if( !memValue.IsValid() ) {
        return;
    }
    const auto memExpressionValue = GetTypedNode<CBinaryExpression>( memValue->Address() );
    if( !memExpressionValue.IsValid() 
        || memExpressionValue->Operation() != IRTree::TOperatorType::OT_Plus )
    {
        return;
    }
    const IRTree::CExpression* left = memExpressionValue->LeftOperand();
    const IRTree::CExpression* right = memExpressionValue->RightOperand();

    const auto leftConst = GetTypedNode<CConstExpression>( left );
    if( !leftConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( right ) + GetDynamicPrice( source ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CStatement>( new CStoreCommand(
                GetDynamicValue( dest ),
                leftConst->Value(),
                GetDynamicValue( source )
            ) ) );
    }

}

void CStoreMemoryLeftOffsetPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMoveStatement>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* dest = root->Destination();
    const IRTree::CExpression* source = root->Source();

    const auto memValue = GetTypedNode<CMemExpression>( dest );
    if( !memValue.IsValid() ) {
        return;
    }
    const auto memExpressionValue = GetTypedNode<CBinaryExpression>( memValue->Address() );
    if( !memExpressionValue.IsValid() 
        || memExpressionValue->Operation() != IRTree::TOperatorType::OT_Plus )
    {
        return;
    }
    const IRTree::CExpression* left = memExpressionValue->LeftOperand();
    const IRTree::CExpression* right = memExpressionValue->RightOperand();

    const auto rightConst = GetTypedNode<CConstExpression>( right );
    if( !rightConst.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( left ) + GetDynamicPrice( source ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CStatement>( new CStoreCommand(
                GetDynamicValue( dest ),
                rightConst->Value(),
                GetDynamicValue( source )
            ) ) );
    }
}

void CStoreRegisterPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CMoveStatement>( node );
    if( !root.IsValid() ) {
        return;
    }
    const IRTree::CExpression* dest = root->Destination();
    const IRTree::CExpression* source = root->Source();

    const auto destValue = GetTypedNode<IRTree::CTempExpression>( dest );
    if( !destValue.IsValid() ) {
        return;
    }

    const int price = GetDynamicPrice( source ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price, 
            std::unique_ptr<const CStatement>( new CMoveRegisterCommand( 
                new Synthesis::CTempExpression( destValue->Temporary().ToString() ) ,
                GetDynamicValue( source )
            ) ) );
    }
}

void CCallFunctionPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CCallExpression>( node );

    if( !root.IsValid() ) {
        return;
    }

    const auto function = GetTypedNode<CNameExpression>( root->Function() );
    const std::vector< std::unique_ptr<const IRTree::CExpression> >& arguments 
        = root->Arguments()->Expressions();
    
    int price = 1;

    std::vector<const Synthesis::CExpression*> commandArguments;
    for( unsigned int i = 0; i < arguments.size(); i++ ) {
        price += GetDynamicPrice( arguments[i].get() );
        commandArguments.push_back( GetDynamicValue( arguments[i].get() ) );
    }
    (*dynamic)[*root] = std::make_pair( price, 
        std::unique_ptr<const CExpression>( new CCallFunctionCommand(
            function->Label().ToString(), commandArguments) ) );
}

void CJumpPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CJumpStatement>( node );

    if( !root.IsValid() ) {
        return;
    }

    std::string targetLabel = root->Target().ToString();

    const int price = 1;

    (*dynamic)[*root] = std::make_pair( price, std::unique_ptr<const CStatement>( new CJumpCommand( targetLabel ) ) );
}

void CConditionalJumpPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CJumpConditionalStatement>( node );

    if( !root.IsValid() ) {
        return;
    }

    const IRTree::CExpression* leftOperand = root->LeftOperand();
    const IRTree::CExpression* rightOperand = root->RightOperand();

    const int price = GetDynamicPrice( leftOperand ) + GetDynamicPrice( rightOperand ) + 1;

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price, 
            std::unique_ptr<const CStatement>( new CConditionalJumpCommand( GetDynamicValue( leftOperand ), 
                GetDynamicValue( rightOperand ), root->Operation(), root->TrueLabel().ToString(), 
                root->FalseLabel().ToString() ) ) );
    }
}

void CExpStatementPattern::Consume( const IRTVT* node ) {
    const auto root = GetTypedNode<CExpStatement>( node );

    if ( !root.IsValid() ) {
        return;
    }

    const IRTree::CExpression* expression = root->Expression();

    const int price = GetDynamicPrice( expression );

    if( GetDynamicPrice( *root ) > price ) {
        (*dynamic)[*root] = std::make_pair( price,
            std::unique_ptr<const CStatement>( new CExpStatementCommand( GetDynamicValue( expression ) ) )
        );
    }
}
