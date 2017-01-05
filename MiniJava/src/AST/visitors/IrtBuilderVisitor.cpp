#include <AST/visitors/IrtBuilderVisitor.h>

using namespace AstTree;

IRTree::TOperatorType CIrtBuilderVisitor::operatorFromAstToIr( TOperatorType type ) const {
    IRTree::TOperatorType typeResult;
    switch ( type ) {
        case TOperatorType::OT_Plus: typeResult = IRTree::TOperatorType::OT_Plus;
        case TOperatorType::OT_Minus: typeResult = IRTree::TOperatorType::OT_Minus;
        case TOperatorType::OT_Times: typeResult = IRTree::TOperatorType::OT_Times;
        case TOperatorType::OT_Div: typeResult = IRTree::TOperatorType::OT_Div;
        case TOperatorType::OT_Mod: typeResult = IRTree::TOperatorType::OT_Mod;
        case TOperatorType::OT_And: typeResult = IRTree::TOperatorType::OT_And;
        case TOperatorType::OT_Or: typeResult = IRTree::TOperatorType::OT_Or;
        default: {
            // such cases should never happen
            assert( false ) ;
        }
    }
    return typeResult;
}

/*__________ Access Modifiers __________*/

void CIrtBuilderVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CIrtBuilderVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept( this );
    const IRTree::CExpression* expressionLeft = subtreeWrapper->ToExpression();

    expression->RightOperand()->Accept( this );
    const IRTree::CExpression* expressionRight = subtreeWrapper->ToExpression();

    if ( expression->Operation() == TOperatorType::OT_LT ) {
        subtreeWrapper = std::unique_ptr<IRTree::ISubtreeWrapper>( new IRTree::CRelativeConditionalWrapper(
            IRTree::TLogicOperatorType::LOT_LT,
            expressionLeft,
            expressionRight
        ) );
    } else {
        IRTree::TOperatorType operatorType = operatorFromAstToIr( expression->Operation() );

        subtreeWrapper = std::unique_ptr<IRTree::ISubtreeWrapper>( new IRTree::CExpressionWrapper(
            new IRTree::CBinaryExpression( operatorType, expressionLeft, expressionRight )
        ) );
    }

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_ID );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CIrtBuilderVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CIrtBuilderVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CIrtBuilderVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CIrtBuilderVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}
