#include <AST/visitors/IrtBuilderVisitor.h>
#include <IRT/Label.h>

using namespace ASTree;

using TMethodToIRTMap = std::unordered_map<std::string, std::shared_ptr<const IRTree::CStatement>>;

const TMethodToIRTMap* CIrtBuilderVisitor::MethodTrees() const {
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
        const std::unordered_set<std::string>& fields ) {
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

    std::unordered_set<std::string> fieldsNames;
    std::shared_ptr<const CClassDefinition> baseClass = classDefinition;
    while ( baseClass ) {
        auto fields = baseClass->Fields();
        for ( auto it = fields.begin(); it != fields.end(); ++it ) {
            fieldsNames.insert( it->first );
        }
        baseClass = baseClass->HasParent() ? symbolTable->GetClassDefinition( baseClass->GetParentName() ) : nullptr;
    }

    buildNewFrame( classCurrentName, declaration->MethodId()->Name(), argumentsNames, localsNames, fieldsNames );
}

void CIrtBuilderVisitor::buildNewFrame( const CMainClass* mainClass ) {
    std::vector<std::string> emptyVector;
    std::unordered_set<std::string> emptySet;
    buildNewFrame( mainClass->ClassName()->Name(), "main", emptyVector, emptyVector, emptySet );
}

/*__________ Access Modifiers __________*/

void CIrtBuilderVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName, modifier->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, modifier->Location() );
}

void CIrtBuilderVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName, modifier->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, modifier->Location() );
}

/*__________ Expressions __________*/

void CIrtBuilderVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName, expression->Location() );

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

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName, expression->Location() );

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

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName, expression->Location() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CConstExpression( expression->Value() )
    ) );

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName, expression->Location() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CConstExpression( expression->Value() ? 1 : 0 )
    ) );

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ID );
    onNodeEnter( nodeName, expression->Location() );

    const IRTree::IAddress* address = frameCurrent->GetAddress( expression->Name() );

    if ( address ) {
        // expression is a name of local var / argument / field
        updateSubtreeWrapper( new IRTree::CExpressionWrapper(
            new IRTree::CMemExpression(
                address->ToExpression(
                    std::move( std::unique_ptr<IRTree::CExpression>(
                        new IRTree::CTempExpression( frameCurrent->FramePointer() )
                    ) )
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

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName, expression->Location() );

    expression->LengthTarget()->Accept( this );
    const IRTree::CExpression* targetExpression = subtreeWrapper->ToExpression();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CMemExpression(
            targetExpression
        )
    ) );

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_METHOD );
    onNodeEnter( nodeName, expression->Location() );

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
    std::shared_ptr<const CMethodDefinition> methodDefinition = symbolTable->SearchClassHierarchyForMethod( expression->MethodId()->Name(), classDefinition );
    CTypeIdentifier type = methodDefinition->ReturnType();
    if ( type.Type() == TTypeIdentifier::ClassId ) {
        methodCallerClassName = type.ClassName();
    }

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_THIS );
    onNodeEnter( nodeName, expression->Location() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        frameCurrent->GetThis()->ToExpression(
            std::move( std::unique_ptr<const IRTree::CExpression>(
                new IRTree::CTempExpression( frameCurrent->FramePointer() )
            ) )
        )
    ) );
    methodCallerClassName = classCurrentName;

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName, expression->Location() );

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

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName, expression->Location() );

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

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName, expression->Location() );

    expression->TargetExpression()->Accept( this );

    updateSubtreeWrapper( new IRTree::CNegateConditionalWrapper(
        std::move( subtreeWrapper )
    ) );

    onNodeExit( nodeName, expression->Location() );
}

/*__________ Statements __________*/

void CIrtBuilderVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName, statement->Location() );

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

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName, statement->Location() );

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

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_PRINT );
    onNodeEnter( nodeName, statement->Location() );

    statement->PrintTarget()->Accept( this );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        frameCurrent->ExternalCall("print", new IRTree::CExpressionList( subtreeWrapper->ToExpression() ) )
    ) );

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName, statement->Location() );

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

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName, statement->Location() );

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

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_BRACES );
    onNodeEnter( nodeName, statement->Location() );

    statement->List()->Accept( this );

    onNodeExit( nodeName, statement->Location() );
}

/*__________ Type Modifiers __________*/

void CIrtBuilderVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName, typeModifier->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, typeModifier->Location() );
}

void CIrtBuilderVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName, typeModifier->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, typeModifier->Location() );
}

void CIrtBuilderVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName, typeModifier->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, typeModifier->Location() );
}

void CIrtBuilderVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName, typeModifier->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, typeModifier->Location() );
}

/*__________ Other (except lists) __________*/

void CIrtBuilderVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, declaration->Location() );
}

void CIrtBuilderVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG );
    onNodeEnter( nodeName, argument->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, argument->Location() );
}

void CIrtBuilderVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    buildNewFrame( declaration );
    std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );

    declaration->Statements()->Accept( this );
    std::unique_ptr<const IRTree::ISubtreeWrapper> statementListWrapper = std::move( subtreeWrapper );

    declaration->ReturnExpression()->Accept( this );
    const IRTree::CExpression* expressionReturn = subtreeWrapper->ToExpression();

    if ( statementListWrapper ) {
        updateSubtreeWrapper( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
                new IRTree::CSeqStatement(
                    statementListWrapper->ToStatement(),
                    new IRTree::CMoveStatement(
                        new IRTree::CTempExpression( frameCurrent->ReturnValueTemp() ),
                        expressionReturn
                    )
                )
            )
        ) );
    } else {
        updateSubtreeWrapper( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
                new IRTree::CMoveStatement(
                    new IRTree::CTempExpression( frameCurrent->ReturnValueTemp() ),
                    expressionReturn
                )
            )
        ) );
    }

    onNodeExit( nodeName, declaration->Location() );
}

void CIrtBuilderVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName, mainClass->Location() );

    buildNewFrame( mainClass );
    std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );

    mainClass->Statements()->Accept( this );
    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
            subtreeWrapper->ToStatement()
        )
    ) );

    onNodeExit( nodeName, mainClass->Location() );
}

void CIrtBuilderVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    classCurrentName = declaration->ClassName()->Name();
    declaration->MethodDeclarations()->Accept( this );

    onNodeExit( nodeName, declaration->Location() );
}

void CIrtBuilderVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CNodeNames::PROGRAM );
    onNodeEnter( nodeName, program->Location() );

    program->MainClass()->Accept( this );
    program->ClassDeclarations()->Accept( this );

    onNodeExit( nodeName, program->Location() );
}

/*__________  Lists __________*/

void CIrtBuilderVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName, list->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, list->Location() );
}

void CIrtBuilderVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();

    std::unique_ptr<const IRTree::ISubtreeWrapper> resultOnSuffix = nullptr;
    if ( !statements.empty() ) {
        // statements must be reversed before being used
        // we'll actually iterate over them in reversed order (the last statement will be the first)
        ( statements.front() )->Accept( this );
        resultOnSuffix = std::move( subtreeWrapper );
        for ( auto it = std::next( statements.begin() ); it != statements.end(); ++it ) {
            ( *it )->Accept( this );
            std::unique_ptr<const IRTree::ISubtreeWrapper> resultCurrent = std::move( subtreeWrapper );
            resultOnSuffix = std::unique_ptr<const IRTree::ISubtreeWrapper>( new IRTree::CStatementWrapper(
                new IRTree::CSeqStatement(
                    resultCurrent->ToStatement(),
                    resultOnSuffix->ToStatement()
                )
            ) );
        }
    }

    subtreeWrapper = std::move( resultOnSuffix );

    onNodeExit( nodeName, list->Location() );
}

void CIrtBuilderVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, list->Location() );
}

void CIrtBuilderVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName, list->Location() );

    // such calls should never happen
    assert( false );

    onNodeExit( nodeName, list->Location() );
}

void CIrtBuilderVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methods = list->MethodDeclarations();

    for ( auto it = methods.begin(); it != methods.end(); ++it ) {
        ( *it )->Accept( this );
        subtreeWrapper->ToStatement();
        std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );
        methodTrees->emplace( methodFullName, std::unique_ptr<const IRTree::CStatement>( subtreeWrapper->ToStatement() ) );
    }

    onNodeExit( nodeName, list->Location() );
}

void CIrtBuilderVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classes = list->ClassDeclarations();

    for ( auto it = classes.begin(); it != classes.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName, list->Location() );
}
