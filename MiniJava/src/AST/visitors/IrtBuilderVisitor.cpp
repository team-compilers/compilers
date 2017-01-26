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

void CIrtBuilderVisitor::updateSubtreeWrapper( const IRTree::ISubtreeWrapper* wrapperNew ) {
    subtreeWrapper = std::unique_ptr<const IRTree::ISubtreeWrapper>( wrapperNew );
}

/*__________ Access Modifiers __________*/

void CIrtBuilderVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

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
        updateSubtreeWrapper( new IRTree::CRelativeConditionalWrapper(
            IRTree::TLogicOperatorType::LOT_LT,
            expressionLeft,
            expressionRight
        ) );
    } else {
        IRTree::TOperatorType operatorType = operatorFromAstToIr( expression->Operation() );

        updateSubtreeWrapper( new IRTree::CExpressionWrapper(
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

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CConstExpression( expression->Value() )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CConstExpression( expression->Value() ? 1 : 0 )
    ) );

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

    expression->LengthExpression->Accept( this );

    const IRTree::CExpression* expressionLength = subtreeWrapper->ToExpression();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        frame.ExternalCall("initArray", new IRTree::CExpressionList( expressionLength ) );
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    const IRTree::CExpression* tempExpression = new IRTree::CTempExpression( CTemp() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CEseqExpression(
            new IRTree::CMoveStatement(
                tempExpression,
                frame.ExternalCall("malloc", new IRTree::CExpressionList( IRTree::CConstExpression( /*TODO*/0 ) ))
            )
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    expression->TargetExpression()->Accept( this );

    updateSubtreeWrapper( new IRTree::CNegateConditionalWrapper(
        subtreeWrapper
    ) );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CIrtBuilderVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    statement->LeftPart()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperLeftPart = subtreeWrapper;
    statement->RightPart()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperRightPart = subtreeWrapper;

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new CMoveStatement(
            wrapperLeftPart.ToExpression(),
            wrapperRightPart.ToExpression()
        )
    ) );

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

    statement->Condition()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperCondition = subtreeWrapper;
    statement->PositiveTarget()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperTargetPositive = subtreeWrapper;
    statement->NegativeTarget()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperTargetNegative = subtreeWrapper;

    IRTree::CLabel labelTrue;
    IRTree::CLabel labelFalse;
    IRTree::CLabel labelJoin;

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            wrapperCondition->ToCondition( labelTrue, labelFalse ),
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( labelTrue ),
                new IRTree::CSeqStatement(
                    wrapperTargetPositive->ToStatement(),
                    new IRTree::CSeqStatement(
                        new IRTree::CJumpStatement( labelJoin ),
                        new IRTree::CSeqStatement(
                            new IRTree::CLabelStatement( labelFalse ),
                            new IRTree::CSeqStatement(
                                wrapperTargetNegative->ToStatement(),
                                new IRTree::CLabelStatement( IRTree::CLabel() )
                            )
                        )
                    )
                )
            )
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperCondition = subtreeWrapper;
    statement->Body()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperBody = subtreeWrapper;

    IRTree::CLabel labelLoop;
    IRTree::CLabel labelBody;
    IRTree::CLabel labelDone;

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            new IRTree::CLabelStatement( labelLoop ),
            new IRTree::CSeqStatement(
                wrapperCondition->ToCondition( labelBody, labelDone ),
                new IRTree::CSeqStatement(
                    new IRTree::CLabelStatement( labelBody ),
                    new IRTree::CSeqStatement(
                        wrapperBody->ToStatement(),
                        new IRTree::CSeqStatement(
                            new IRTree::CJumpStatement( labelLoop ),
                            new IRTree::CLabelStatement( labelDone )
                        )
                    )
                )
            )
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    statement->List()->Accept( this );

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CIrtBuilderVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CIrtBuilderVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

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
    // create new frame

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

    declaration->MethodDeclarations()->Accept( this );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    program->MainClass()->Accept( this );
    program->ClassDeclarations()->Accept( this );

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

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();

    // std::vector<std::unique_ptr<const IRTree::ISubtreeWrapper>> statementsTranslated;
    // statementsTranslated.reserve( statements.size() );



    // statements must be reversed before being used
    // we'll actually iterate over it in reversed order (the last statement will be the first)
    ( statements.front() )->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> resultOnSuffix = subtreeWrapper;
    for ( it = std::next( statements.begin() ); it != statements.end(); ++it ) {
        ( *it )->Accept( this );
        std::unique_ptr<const IRTree::ISubtreeWrapper> resultCurrent = subtreeWrapper;
        resultOnSuffix = std::unique_ptr< const IRTree::ISubtreeWrapper >( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                resultCurrent->ToStatement(),
                resultOnSuffix->ToStatement()
            )
        ) );
    }

    subtreeWrapper = resultOnSuffix;

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    auto methods = list->MethodDeclarations();

    for (int i = 0; i < methods.size(); ++i) {
        methods[i]->Accept( this );
        methodTrees["C$M" + std::to_string( i )] = subtreeWrapper->ToStatement();
    }

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classes = list->ClassDeclarations();

    for (int i = 0; i < classes.size(); ++i) {
        classes[i]->Accept( this );
    }

    onNodeExit( nodeName );
}
