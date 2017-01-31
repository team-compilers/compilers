#include <AST/visitors/IrtBuilderVisitor.h>
#include <IRT/Label.h>

using namespace AstTree;

const std::unordered_map<std::string, std::unique_ptr<const IRTree::CStatement>>& CIrtBuilderVisitor::MethodTrees() const {
    return methodTrees;
}

IRTree::TOperatorType CIrtBuilderVisitor::operatorFromAstToIr( TOperatorType type ) const {
    IRTree::TOperatorType typeResult;
    switch ( type ) {
        case TOperatorType::OT_Plus: typeResult = IRTree::TOperatorType::OT_Plus; break;
        case TOperatorType::OT_Minus: typeResult = IRTree::TOperatorType::OT_Minus; break;
        case TOperatorType::OT_Times: typeResult = IRTree::TOperatorType::OT_Times; break;
        case TOperatorType::OT_Div: typeResult = IRTree::TOperatorType::OT_Div; break;
        case TOperatorType::OT_Mod: typeResult = IRTree::TOperatorType::OT_Mod; break;
        case TOperatorType::OT_And: typeResult = IRTree::TOperatorType::OT_And; break;
        case TOperatorType::OT_Or: typeResult = IRTree::TOperatorType::OT_Or; break;
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

std::string CIrtBuilderVisitor::makeMethodFullName( const std::string& className, const std::string& methodName ) {
    return className + "$" + methodName;
}

void CIrtBuilderVisitor::buildNewFrame( const std::string& className, const std::string& methodName,
        const std::vector<std::string>& arguments, const std::vector<std::string>& locals,
        const std::vector<std::string>& fields ) {
    frameCurrent = std::unique_ptr<IRTree::CFrame>( new IRTree::CFrame( className, methodName ) );


    frameCurrent->AddThis();
    for ( auto it = fields.begin(); it != fields.end(); ++it ) {
        frameCurrent->AddField( *it );
    }
    for ( auto it = arguments.begin(); it != arguments.end(); ++it ) {
        frameCurrent->AddArgument( *it );
    }
    frameCurrent->AddReturn();
    for ( auto it = locals.begin(); it != locals.end(); ++it ) {
        frameCurrent->AddLocal( *it );
    }

    std::string methodFullName = makeMethodFullName( className, methodName );
    frames.emplace( methodFullName, frameCurrent );
}

void CIrtBuilderVisitor::buildNewFrame( const CMethodDeclaration* declaration ) {
    std::shared_ptr<const CClassDefinition> classDefinition = symbolTable->GetClassDefinition( classCurrentName );
    std::shared_ptr<const CMethodDefinition> methodDefinition = classDefinition->GetMethodDefinition( declaration->MethodId()->Name() );

    const std::vector<std::unique_ptr<const CMethodArgument>>& arguments = declaration->MethodArguments()->MethodArguments();
    std::vector<std::string> argumentsNames;
    argumentsNames.reserve( arguments.size() );
    for ( auto it = arguments.begin(); it != arguments.end(); ++it ) {
        argumentsNames.push_back( ( *it )->Id()->Name() );
    }

    auto locals = methodDefinition->LocalVariables();
    std::vector<std::string> localsNames;
    localsNames.reserve( locals->size() );
    for ( auto it = locals->begin(); it != locals->end(); ++it ) {
        localsNames.push_back( it->first );
    }

    auto fields = classDefinition->Fields();
    std::vector<std::string> fieldsNames;
    fieldsNames.reserve( fields.size() );
    for ( auto it = fields.begin(); it != fields.end(); ++it ) {
        fieldsNames.push_back( it->first );
    }

    buildNewFrame( classCurrentName, declaration->MethodId()->Name(), argumentsNames, localsNames, fieldsNames );
}

void CIrtBuilderVisitor::buildNewFrame( const CMainClass* mainClass ) {
    std::vector<std::string> emptyVector;
    buildNewFrame( mainClass->ClassName()->Name(), "main", emptyVector, emptyVector, emptyVector );
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

    expression->ContainerExpression()->Accept( this );
    const IRTree::CExpression* containerExpression = subtreeWrapper->ToExpression();

    expression->IndexExpression()->Accept( this );
    const IRTree::CExpression* indexExpression = subtreeWrapper->ToExpression();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CMemExpression(
            new IRTree::CBinaryExpression(
                IRTree::TOperatorType::OT_Plus,
                containerExpression,
                new IRTree::CBinaryExpression(
                    IRTree::TOperatorType::OT_Times,
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Plus,
                        indexExpression,
                        new IRTree::CConstExpression( 1 )
                    ),
                    new IRTree::CConstExpression( frameCurrent->WordSize() )
                )
            )
        )
    ) );

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

    const IRTree::IAddress* address = frameCurrent->GetAddress( expression->Name() );

    if ( address ) {
        // expression is a name of local var / argument / field
        updateSubtreeWrapper( new IRTree::CExpressionWrapper(
            new IRTree::CMemExpression(
                address->ToExpression(
                    new IRTree::CTempExpression( frameCurrent->FramePointer() )
                )
            )
        ) );

        std::shared_ptr<const CClassDefinition> classDefinition = symbolTable->GetClassDefinition( frameCurrent->GetClassName() );
        std::shared_ptr<const CMethodDefinition> methodDefinition = classDefinition->GetMethodDefinition( frameCurrent->GetMethodName() );
        CTypeIdentifier type = methodDefinition->GetVariableType( expression->Name() );
        if ( type.Type() == TTypeIdentifier::NotFound ) {
            type = classDefinition->GetFieldType( expression->Name() );
        }
        if ( type.Type() == TTypeIdentifier::ClassId ) {
            methodCallerClassName = type.ClassName();
        }
    }

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    expression->LengthTarget()->Accept( this );
    const IRTree::CExpression* targetExpression = subtreeWrapper->ToExpression();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CMemExpression(
            targetExpression
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    expression->CallerExpression()->Accept( this );
    std::string methodCaller = methodCallerClassName;

    IRTree::CExpressionList* expressionListIrt = new IRTree::CExpressionList();
    const std::vector< std::unique_ptr<const CExpression> >& expressionsAst = expression->Arguments()->Expressions();
    for ( auto it = expressionsAst.begin(); it != expressionsAst.end(); ++it ) {
        ( *it )->Accept( this );
        expressionListIrt->Add( subtreeWrapper->ToExpression() );
    }

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CCallExpression(
            new IRTree::CNameExpression(
                IRTree::CLabel( makeMethodFullName( methodCaller, expression->MethodId()->Name() ) )
            ),
            expressionListIrt
        )
    ) );

    std::shared_ptr<const CClassDefinition> classDefinition = symbolTable->GetClassDefinition( methodCaller );
    std::shared_ptr<const CMethodDefinition> methodDefinition = classDefinition->GetMethodDefinition( expression->MethodId()->Name() );
    CTypeIdentifier type = methodDefinition->ReturnType();
    if ( type.Type() == TTypeIdentifier::ClassId ) {
        methodCallerClassName = type.ClassName();
    }

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    methodCallerClassName = classCurrentName;

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    expression->LengthExpression()->Accept( this );

    const IRTree::CExpression* expressionLength = subtreeWrapper->ToExpression();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        frameCurrent->ExternalCall(
            "malloc",
            new IRTree::CExpressionList(
                new IRTree::CBinaryExpression(
                    IRTree::TOperatorType::OT_Times,
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Plus,
                        expressionLength,
                        new IRTree::CConstExpression( 1 )
                    ),
                    new IRTree::CConstExpression( frameCurrent->WordSize() )
                )
            )
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    std::shared_ptr<const CClassDefinition> classDefinition = symbolTable->GetClassDefinition( expression->TargetId()->Name() );
    int fieldCount = classDefinition->Fields().size();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        frameCurrent->ExternalCall(
            "malloc",
            new IRTree::CExpressionList(
                new IRTree::CBinaryExpression(
                    IRTree::TOperatorType::OT_Times,
                    new IRTree::CConstExpression( fieldCount ),
                    new IRTree::CConstExpression( frameCurrent->WordSize() )
                )
            )
        )
    ) );

    methodCallerClassName = expression->TargetId()->Name();

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    expression->TargetExpression()->Accept( this );

    updateSubtreeWrapper( new IRTree::CNegateConditionalWrapper(
        subtreeWrapper.get()
    ) );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CIrtBuilderVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    statement->LeftPart()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperLeftPart = std::move( subtreeWrapper );
    statement->RightPart()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperRightPart = std::move( subtreeWrapper );

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CMoveStatement(
            wrapperLeftPart->ToExpression(),
            wrapperRightPart->ToExpression()
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    statement->LeftPartId()->Accept( this );
    const IRTree::CExpression* leftPartExpression = subtreeWrapper->ToExpression();

    statement->RightPart()->Accept( this );
    const IRTree::CExpression* rightPartExpression = subtreeWrapper->ToExpression();

    statement->LeftPartIndex()->Accept( this );
    const IRTree::CExpression* indexExpression = subtreeWrapper->ToExpression();

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CMoveStatement(
            new IRTree::CMemExpression(
                new IRTree::CBinaryExpression(
                    IRTree::TOperatorType::OT_Plus,
                    leftPartExpression,
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Times,
                        new IRTree::CBinaryExpression(
                            IRTree::TOperatorType::OT_Plus,
                            indexExpression,
                            new IRTree::CConstExpression( 1 )
                        ),
                        new IRTree::CConstExpression( frameCurrent->WordSize() )
                    )
                )
            ),
            rightPartExpression
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    statement->PrintTarget()->Accept( this );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        frameCurrent->ExternalCall("print", new IRTree::CExpressionList( subtreeWrapper->ToExpression() ) )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperCondition = std::move( subtreeWrapper );
    statement->PositiveTarget()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperTargetPositive = std::move( subtreeWrapper );
    statement->NegativeTarget()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperTargetNegative = std::move( subtreeWrapper );

    IRTree::CLabel labelTrue;
    IRTree::CLabel labelFalse;
    IRTree::CLabel labelJoin;

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            wrapperCondition->ToConditional( labelTrue, labelFalse ),
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
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperCondition = std::move( subtreeWrapper );
    statement->Body()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> wrapperBody = std::move( subtreeWrapper );

    IRTree::CLabel labelLoop;
    IRTree::CLabel labelBody;
    IRTree::CLabel labelDone;

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            new IRTree::CLabelStatement( labelLoop ),
            new IRTree::CSeqStatement(
                wrapperCondition->ToConditional( labelBody, labelDone ),
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

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

    buildNewFrame( declaration );
    std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );

    declaration->Statements()->Accept( this );
    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
            subtreeWrapper->ToStatement()
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );

    buildNewFrame( mainClass );
    std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );

    mainClass->Statements()->Accept( this );
    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
            subtreeWrapper->ToStatement()
        )
    ) );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL );
    onNodeEnter( nodeName );

    classCurrentName = declaration->ClassName()->Name();
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

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();

    // statements must be reversed before being used
    // we'll actually iterate over it in reversed order (the last statement will be the first)
    ( statements.front() )->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> resultOnSuffix = std::move( subtreeWrapper );
    for ( auto it = std::next( statements.begin() ); it != statements.end(); ++it ) {
        ( *it )->Accept( this );
        std::unique_ptr<const IRTree::ISubtreeWrapper> resultCurrent = std::move( subtreeWrapper );
        resultOnSuffix = std::unique_ptr< const IRTree::ISubtreeWrapper >( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                resultCurrent->ToStatement(),
                resultOnSuffix->ToStatement()
            )
        ) );
    }

    subtreeWrapper = std::move( resultOnSuffix );

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

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methods = list->MethodDeclarations();

    for ( auto it = methods.begin(); it != methods.end(); ++it ) {
        ( *it )->Accept( this );
        subtreeWrapper->ToStatement();
        std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );
        methodTrees[methodFullName] = std::unique_ptr<const IRTree::CStatement>( subtreeWrapper->ToStatement() );
    }

    onNodeExit( nodeName );
}

void CIrtBuilderVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classes = list->ClassDeclarations();

    for ( auto it = classes.begin(); it != classes.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}
