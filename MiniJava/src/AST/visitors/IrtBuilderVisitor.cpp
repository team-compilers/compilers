#include <AST/visitors/IrtBuilderVisitor.h>
#include <IRT/Label.h>

using namespace ASTree;

using TMethodToIRTMap = std::unordered_map<std::string, std::unique_ptr<const IRTree::CStatement>>;

std::unique_ptr<const TMethodToIRTMap> CIrtBuilderVisitor::MethodTrees() {
    return std::unique_ptr<const TMethodToIRTMap>( methodTrees.release() );
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

void CIrtBuilderVisitor::updateSubtreeWrapper( IRTree::ISubtreeWrapper* wrapperNew ) {
    subtreeWrapper = std::unique_ptr<IRTree::ISubtreeWrapper>( wrapperNew );
}

void CIrtBuilderVisitor::updateSubtreeWrapper( std::unique_ptr<IRTree::ISubtreeWrapper> wrapperNew ) {
    subtreeWrapper = std::move( wrapperNew );
}

std::string CIrtBuilderVisitor::makeMethodFullName( const std::string& className, const std::string& methodName ) {
    return className + "@" + methodName;
}

template <class InputIteratorArguments, class InputIteratorLocals, class InputIteratorFields>
void CIrtBuilderVisitor::buildNewFrame( const std::string& className, const std::string& methodName,
        InputIteratorArguments argumentsLeftIt, InputIteratorArguments argumentsRightIt,
        InputIteratorLocals localsLeftIt, InputIteratorLocals localsRightIt,
        InputIteratorFields fieldsLeftIt, InputIteratorFields fieldsRightIt ) {
    std::unique_ptr<IRTree::CFrame> frameNew = std::unique_ptr<IRTree::CFrame>( new IRTree::CFrame( className, methodName ) );

    frameCurrent = frameNew.get();

    frameCurrent->AddThisAddress();
    for ( auto it = fieldsLeftIt; it != fieldsRightIt; ++it ) {
        frameCurrent->AddFieldAddress( *it );
    }
    // arguments and locals should be added after fields
    // in order to overwrite them in the map of addresses in case of name collision
    for ( auto it = argumentsLeftIt; it != argumentsRightIt; ++it ) {
        frameCurrent->AddArgumentAddress( *it );
    }
    frameCurrent->AddReturnAddress();
    for ( auto it = localsLeftIt; it != localsRightIt; ++it ) {
        frameCurrent->AddLocalAddress( *it );
    }

    std::string methodFullName = makeMethodFullName( className, methodName );
    frames.emplace( methodFullName, std::move( frameNew ) );
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

    buildNewFrame( classCurrentName, declaration->MethodId()->Name(), argumentsNames.begin(), argumentsNames.end(),
        localsNames.begin(), localsNames.end(), fieldsNames.begin(), fieldsNames.end() );
}

void CIrtBuilderVisitor::buildNewFrame( const CMainClass* mainClass ) {
    std::unordered_set<std::string> emptySet;
    buildNewFrame( mainClass->ClassName()->Name(), "main", emptySet.end(), emptySet.end(),
        emptySet.end(), emptySet.end(), emptySet.end(), emptySet.end() );
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
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperLeft = std::move( subtreeWrapper );

    expression->RightOperand()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperRight = std::move( subtreeWrapper );

    if ( expression->Operation() == TOperatorType::OT_LT ) {
        updateSubtreeWrapper( new IRTree::CRelativeConditionalWrapper(
            IRTree::TLogicOperatorType::LOT_LT,
            std::move( wrapperLeft->ToExpression() ),
            std::move( wrapperRight->ToExpression() )
        ) );
    } else if ( expression->Operation() == TOperatorType::OT_And ) {
        updateSubtreeWrapper( new IRTree::CAndConditionalWrapper(
            std::move( wrapperLeft ),
            std::move( wrapperRight )
        ) );
    } else if ( expression->Operation() == TOperatorType::OT_Or ) {
        updateSubtreeWrapper( new IRTree::COrConditionalWrapper(
            std::move( wrapperLeft ),
            std::move( wrapperRight )
        ) );
    } else {
        IRTree::TOperatorType operatorType = operatorFromAstToIr( expression->Operation() );

        updateSubtreeWrapper( new IRTree::CExpressionWrapper(
            new IRTree::CBinaryExpression(
                operatorType,
                std::move( wrapperLeft->ToExpression() ),
                std::move( wrapperRight->ToExpression() )
            )
        ) );
    }

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName, expression->Location() );

    expression->ContainerExpression()->Accept( this );
    std::unique_ptr<const IRTree::CExpression> containerExpression = std::move( subtreeWrapper->ToExpression() );

    expression->IndexExpression()->Accept( this );
    std::unique_ptr<const IRTree::CExpression> indexExpression = std::move( subtreeWrapper->ToExpression() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        new IRTree::CMemExpression(
            new IRTree::CBinaryExpression(
                IRTree::TOperatorType::OT_Plus,
                std::move( containerExpression ),
                std::move( std::unique_ptr<const IRTree::CExpression>(
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Times,
                        new IRTree::CBinaryExpression(
                            IRTree::TOperatorType::OT_Plus,
                            std::move( indexExpression ),
                            std::move( std::unique_ptr<const IRTree::CExpression>(
                                new IRTree::CConstExpression( 1 )
                            ) )
                        ),
                        new IRTree::CConstExpression( frameCurrent->GetWordSize() )
                    )
                ) )
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

        std::shared_ptr<const CClassDefinition> classDefinition = symbolTable->GetClassDefinition( frameCurrent->GetClassName() );
        std::shared_ptr<const CMethodDefinition> methodDefinition = classDefinition->GetMethodDefinition( frameCurrent->GetMethodName() );
        CTypeIdentifier type = methodDefinition->GetVariableType( expression->Name() );
        if ( type.Type() == TTypeIdentifier::NotFound ) {
            // expression is a name of field
            updateSubtreeWrapper( new IRTree::CExpressionWrapper(
                std::move( address->ToExpression() )
            ) );
            type = classDefinition->GetFieldType( expression->Name() );
        } else {
            // expression is a name of local var / argument
            updateSubtreeWrapper( new IRTree::CExpressionWrapper(
                std::move( address->ToExpression() )
            ) );
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
    std::unique_ptr<const IRTree::CExpression> targetExpression = std::move( subtreeWrapper->ToExpression() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        std::move( targetExpression )
    ) );

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_METHOD );
    onNodeEnter( nodeName, expression->Location() );

    expression->CallerExpression()->Accept( this );
    std::string methodCaller = methodCallerClassName;

    IRTree::CExpressionList* expressionListIrt = new IRTree::CExpressionList();
    expressionListIrt->Add( std::move( subtreeWrapper->ToExpression() ) );

    const std::vector<std::unique_ptr<const CExpression>>& expressionsAst = expression->Arguments()->Expressions();
    for ( auto it = expressionsAst.begin(); it != expressionsAst.end(); ++it ) {
        ( *it )->Accept( this );
        expressionListIrt->Add( std::move( subtreeWrapper->ToExpression() ) );
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
        std::move( frameCurrent->GetThisAddress()->ToExpression() )
    ) );
    methodCallerClassName = classCurrentName;

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName, expression->Location() );

    expression->LengthExpression()->Accept( this );

    std::unique_ptr<const IRTree::CExpression> expressionLength = std::move( subtreeWrapper->ToExpression() );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        std::move( frameCurrent->ExternalCall(
            "malloc",
            std::move( std::unique_ptr<const IRTree::CExpressionList>(
                new IRTree::CExpressionList(
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Times,
                        new IRTree::CBinaryExpression(
                            IRTree::TOperatorType::OT_Plus,
                            std::move( expressionLength ),
                            std::move( std::unique_ptr<IRTree::CConstExpression>( new IRTree::CConstExpression( 1 ) ) )
                        ),
                        new IRTree::CConstExpression( frameCurrent->GetWordSize() )
                    )
                )
            ) )
        ) )
    ) );

    onNodeExit( nodeName, expression->Location() );
}

void CIrtBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName, expression->Location() );

    std::shared_ptr<const CClassDefinition> classDefinition = symbolTable->GetClassDefinition( expression->TargetId()->Name() );
    int fieldCount = classDefinition->Fields().size();

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        std::move( frameCurrent->ExternalCall(
            "malloc",
            std::move( std::unique_ptr<const IRTree::CExpressionList>(
                new IRTree::CExpressionList(
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Times,
                        new IRTree::CConstExpression( fieldCount ),
                        new IRTree::CConstExpression( frameCurrent->GetWordSize() )
                    )
                )
            ) )
        ) )
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
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperLeftPart = std::move( subtreeWrapper );
    statement->RightPart()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperRightPart = std::move( subtreeWrapper );

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CMoveStatement(
            std::move( wrapperLeftPart->ToExpression() ),
            std::move( wrapperRightPart->ToExpression() )
        )
    ) );

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName, statement->Location() );

    statement->LeftPartId()->Accept( this );
    std::unique_ptr<const IRTree::CExpression> leftPartExpression = std::move( subtreeWrapper->ToExpression() );

    statement->RightPart()->Accept( this );
    std::unique_ptr<const IRTree::CExpression> rightPartExpression = std::move( subtreeWrapper->ToExpression() );

    statement->LeftPartIndex()->Accept( this );
    std::unique_ptr<const IRTree::CExpression> indexExpression = std::move( subtreeWrapper->ToExpression() );

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CMoveStatement(
            std::move( std::unique_ptr<const IRTree::CExpression>(
                new IRTree::CMemExpression(
                    new IRTree::CBinaryExpression(
                        IRTree::TOperatorType::OT_Plus,
                        std::move( leftPartExpression ),
                        std::move( std::unique_ptr<const IRTree::CExpression>(
                            new IRTree::CBinaryExpression(
                                IRTree::TOperatorType::OT_Times,
                                new IRTree::CBinaryExpression(
                                    IRTree::TOperatorType::OT_Plus,
                                    std::move( indexExpression ),
                                    std::move( std::unique_ptr<const IRTree::CExpression>(
                                        new IRTree::CConstExpression( 1 )
                                    ) )
                                ),
                                new IRTree::CConstExpression( frameCurrent->GetWordSize() )
                            )
                        ) )
                    )
                )
            ) ),
            std::move( rightPartExpression )
        )
    ) );

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_PRINT );
    onNodeEnter( nodeName, statement->Location() );

    statement->PrintTarget()->Accept( this );

    updateSubtreeWrapper( new IRTree::CExpressionWrapper(
        std::move( frameCurrent->ExternalCall(
            "print",
            std::move( std::unique_ptr<const IRTree::CExpressionList>(
                new IRTree::CExpressionList( std::move( subtreeWrapper->ToExpression() ) )
            ) )
        ) )
    ) );

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName, statement->Location() );

    statement->Condition()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperCondition = std::move( subtreeWrapper );
    statement->PositiveTarget()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperTargetPositive = std::move( subtreeWrapper );
    statement->NegativeTarget()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperTargetNegative = std::move( subtreeWrapper );

    IRTree::CLabel labelTrue;
    IRTree::CLabel labelFalse;
    IRTree::CLabel labelJoin;

    IRTree::CLabel* resultLabelTrue = &labelJoin;
    IRTree::CLabel* resultLabelFalse = &labelJoin;

    std::unique_ptr<const IRTree::CStatement> suffix( new IRTree::CLabelStatement( labelJoin ) );
    if ( wrapperTargetNegative ) {
        resultLabelFalse = &labelFalse;

        suffix = std::move( std::unique_ptr<const IRTree::CStatement>(
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( labelFalse ),
                new IRTree::CSeqStatement(
                    std::move( wrapperTargetNegative->ToStatement() ),
                    std::move( suffix )
                )
            )
        ) );
        if ( wrapperTargetPositive ) {
            suffix = std::move( std::unique_ptr<const IRTree::CStatement>(
                new IRTree::CSeqStatement(
                    std::move( std::unique_ptr<const IRTree::CStatement>(
                        new IRTree::CJumpStatement( labelJoin )
                    ) ),
                    std::move( suffix )
                )
            ) );
        }
    }

    if ( wrapperTargetPositive ) {
        resultLabelTrue = &labelTrue;

        suffix = std::move( std::unique_ptr<const IRTree::CStatement>(
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( labelTrue ),
                new IRTree::CSeqStatement(
                    std::move( wrapperTargetPositive->ToStatement() ),
                    std::move( suffix )
                )
            )
        ) );
    }

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            std::move( wrapperCondition->ToConditional( *resultLabelTrue, *resultLabelFalse ) ),
            std::move( suffix )
        )
    ) );

    onNodeExit( nodeName, statement->Location() );
}

void CIrtBuilderVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName, statement->Location() );

    statement->Condition()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperCondition = std::move( subtreeWrapper );
    statement->Body()->Accept( this );
    std::unique_ptr<IRTree::ISubtreeWrapper> wrapperBody = std::move( subtreeWrapper );

    IRTree::CLabel labelLoop;
    IRTree::CLabel labelBody;
    IRTree::CLabel labelDone;

    std::unique_ptr<const IRTree::CStatement> suffix(
        new IRTree::CSeqStatement(
            new IRTree::CJumpStatement( labelLoop ),
            new IRTree::CLabelStatement( labelDone )
        )
    );
    if ( wrapperBody ) {
        suffix = std::move( std::unique_ptr<const IRTree::CStatement>(
            new IRTree::CSeqStatement(
                std::move( wrapperBody->ToStatement() ),
                std::move( suffix )
            )
        ) );
    }

    updateSubtreeWrapper( new IRTree::CStatementWrapper(
        new IRTree::CSeqStatement(
            new IRTree::CLabelStatement( labelLoop ),
            new IRTree::CSeqStatement(
                std::move( wrapperCondition->ToConditional( labelBody, labelDone ) ),
                std::move( std::unique_ptr<const IRTree::CStatement>(
                    new IRTree::CSeqStatement(
                        std::move( std::unique_ptr<const IRTree::CStatement>(
                            new IRTree::CLabelStatement( labelBody )
                        ) ),
                        std::move( suffix )
                    )
                ) )
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
    std::unique_ptr<IRTree::ISubtreeWrapper> statementListWrapper = std::move( subtreeWrapper );

    declaration->ReturnExpression()->Accept( this );
    std::unique_ptr<const IRTree::CExpression> expressionReturn = std::move( subtreeWrapper->ToExpression() );

    if ( statementListWrapper ) {
        updateSubtreeWrapper( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
                new IRTree::CSeqStatement(
                    std::move( statementListWrapper->ToStatement() ),
                    std::move( std::unique_ptr<const IRTree::CStatement>(
                        new IRTree::CMoveStatement(
                            std::move( frameCurrent->GetReturnValueAddress()->ToExpression() ),
                            std::move( expressionReturn )
                        )
                    ) )
                )
            )
        ) );
    } else {
        updateSubtreeWrapper( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) ),
                new IRTree::CMoveStatement(
                    std::move( frameCurrent->GetReturnValueAddress()->ToExpression() ),
                    std::move( expressionReturn )
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
    std::unique_ptr<IRTree::ISubtreeWrapper> statementListWrapper = std::move( subtreeWrapper );
    if ( statementListWrapper ) {
        updateSubtreeWrapper( new IRTree::CStatementWrapper(
            new IRTree::CSeqStatement(
                std::move( std::unique_ptr<const IRTree::CStatement>(
                    new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) )
                ) ),
                std::move( statementListWrapper->ToStatement() )
            )
        ) );
    } else {
        // empty function
        updateSubtreeWrapper( new IRTree::CStatementWrapper(
            new IRTree::CLabelStatement( IRTree::CLabel( methodFullName ) )
        ) );
    }

    methodTrees->emplace( methodFullName, std::move( subtreeWrapper->ToStatement() ) );

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

    std::unique_ptr<IRTree::ISubtreeWrapper> resultOnSuffix = nullptr;
    if ( !statements.empty() ) {
        // statements must be reversed before being used
        // we'll actually iterate over them in reversed order (the last statement will be the first)
        ( statements.front() )->Accept( this );
        resultOnSuffix = std::move( subtreeWrapper );
        for ( auto it = std::next( statements.begin() ); it != statements.end(); ++it ) {
            ( *it )->Accept( this );
            std::unique_ptr<IRTree::ISubtreeWrapper> resultCurrent = std::move( subtreeWrapper );
            resultOnSuffix = std::unique_ptr<IRTree::ISubtreeWrapper>( new IRTree::CStatementWrapper(
                new IRTree::CSeqStatement(
                    std::move( resultCurrent->ToStatement() ),
                    std::move( resultOnSuffix->ToStatement() )
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
        std::string methodFullName = makeMethodFullName( frameCurrent->GetClassName(), frameCurrent->GetMethodName() );
        methodTrees->emplace( methodFullName, std::move( subtreeWrapper->ToStatement() ) );
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
