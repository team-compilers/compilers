#include <AST/visitors/TypeCheckerVisitor.h>

using namespace ASTree;

const std::vector<CCompilationError>* CTypeCheckerVisitor::GetErrors() const {
    return errors;
}

/*__________ Access Modifiers __________*/

// ignored
void CTypeCheckerVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName, modifier->Location() );
    onNodeExit( nodeName, modifier->Location() );
}

// ignored
void CTypeCheckerVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName, modifier->Location() );
    onNodeExit( nodeName, modifier->Location() );
}

/*__________ Expressions __________*/

void CTypeCheckerVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName, expression->Location() );

    TTypeIdentifier operatorType; 
    if ( expression->Operation() == TOperatorType::OT_LT ||
         expression->Operation() == TOperatorType::OT_And ||
         expression->Operation() == TOperatorType::OT_Or ) {
        operatorType = TTypeIdentifier::Boolean;
    } else {
        operatorType = TTypeIdentifier::Int;
    }
    expression->LeftOperand()->Accept( this );
    TTypeIdentifier leftOperandType = lastType.front().Type();

    expression->RightOperand()->Accept( this );
    TTypeIdentifier RightOperandType = lastType.front().Type();

    if ( leftOperandType != RightOperandType ) {
        errors->push_back( CCompilationError( ( expression )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
        lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };
    } else {
        lastType = { CTypeIdentifier( operatorType ) };
    }
    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName, expression->Location() );

    expression->ContainerExpression()->Accept( this );

    TTypeIdentifier containerType;
    if ( lastType.front().Type() == TTypeIdentifier::IntArray ) {
        containerType = TTypeIdentifier::Int;
    } else {
        containerType = TTypeIdentifier::NotFound;
    }

    expression->IndexExpression()->Accept( this );

    if ( lastType.front().Type() != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( expression )->Location(), CCompilationError::INVALID_INDEX_TYPE ) );
    }

    lastType = { CTypeIdentifier( containerType ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName, expression->Location() );

    lastType = { CTypeIdentifier( TTypeIdentifier::Int ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName, expression->Location() );

    lastType = { CTypeIdentifier( TTypeIdentifier::Boolean ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ID );
    onNodeEnter( nodeName, expression->Location() );

    std::string name = expression->Name();
    CTypeIdentifier notFound( TTypeIdentifier::NotFound );

    CTypeIdentifier fieldLurk = symbolTablePtr->SearchClassHierarchyForField( name, lastClass );
    // TODO: do we really have to perform methodLurk?
    std::shared_ptr<const CMethodDefinition> methodLurk = symbolTablePtr->SearchClassHierarchyForMethod( name, lastClass );
    CTypeIdentifier localLurk = lastMethod->GetLocalVariableType(name);
    CTypeIdentifier argumentLurk = lastMethod->GetArgumentType(name);

    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    // The latter wins.
    if ( fieldLurk != notFound ) {
        lastType = { fieldLurk };
    }
    if ( methodLurk != nullptr && methodLurk->ReturnType() != notFound ) {
        lastType = { methodLurk->ReturnType() };
    }
    if ( argumentLurk != notFound ) {
        lastType = { argumentLurk };
    }
    if ( localLurk != notFound ) {
        lastType = { localLurk };
    }

    if ( lastType.front().Type() == TTypeIdentifier::NotFound ) {
        errors->push_back( CCompilationError( expression->Location(), CCompilationError::VAR_UNDEFINED ) );
    }

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName, expression->Location() );

    expression->LengthTarget()->Accept ( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::Int ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_METHOD );
    onNodeEnter( nodeName, expression->Location() );

    CTypeIdentifier methodReturnType = CTypeIdentifier( TTypeIdentifier::NotFound );
    expression->CallerExpression()->Accept( this );

    std::string methodName = expression->MethodId()->Name();
    std::string className = lastType.front().ClassName();

    std::shared_ptr<const CClassDefinition> callerClassDefinition;

    callerClassDefinition = symbolTablePtr->GetClassDefinition( className );

    if ( callerClassDefinition == nullptr ) {
        errors->emplace_back( expression->Location(), CCompilationError::INVALID_CALLER_EXPRESSION );
    } else {
        std::shared_ptr<const CMethodDefinition> methodDefinition = symbolTablePtr->SearchClassHierarchyForMethod( methodName, callerClassDefinition );

        if ( methodDefinition == nullptr ) {
            errors->emplace_back( expression->Location(), CCompilationError::CLASS_HAS_NO_METHOD );
            methodReturnType = CTypeIdentifier( TTypeIdentifier::NotFound );
        } else {
            if ( methodDefinition->AccessModifier() == TAccessModifier::Private &&
                 callerClassDefinition != lastClass ) {
                errors->emplace_back( expression->Location(), CCompilationError::METHOD_IS_PRIVATE );
            }

            expression->Arguments()->Accept( this );
            if ( lastType.size() != methodDefinition->GetArgumentsNumber() ) {
                errors->emplace_back( expression->Location(), CCompilationError::ARGS_NUMBERS_NOT_MATCH );
            } else {
                for ( int i = 0; i < lastType.size(); ++i ) {
                    CTypeIdentifier expectedType = methodDefinition->GetArgumentType(i);
                    if ( lastType[i] != expectedType ) {
                        bool canBeResolved = false;
                        if ( lastType[i].Type() == TTypeIdentifier::ClassId &&
                            expectedType.Type() == TTypeIdentifier::ClassId) {

                            std::shared_ptr<const CClassDefinition> ancestor =
                                symbolTablePtr->GetClassDefinition(lastType[i].ClassName());
                            while ( ancestor != nullptr ) {
                                if ( ancestor->ClassName() == expectedType.ClassName() ) {
                                    canBeResolved = true;
                                    break;
                                }
                                if ( ancestor->HasParent() ) {
                                    ancestor = symbolTablePtr->GetClassDefinition(ancestor->GetParentName());
                                } else {
                                    ancestor = nullptr;
                                }
                            }

                        }
                        if ( !canBeResolved ) {
                            errors->emplace_back( expression->Location(), CCompilationError::ARG_TYPE_NOT_MATCH );
                        }
                    }
                }
            }
            methodReturnType = methodDefinition->ReturnType();
        }
    }

    lastType = { methodReturnType };

    onNodeExit( nodeName, expression->Location() );
}


// ignored
void CTypeCheckerVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_THIS );
    onNodeEnter( nodeName, expression->Location() );

    lastType = { CTypeIdentifier( lastClass->ClassName() ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName, expression->Location() );

    expression->LengthExpression()->Accept( this );

    if ( lastType.front().Type() != TTypeIdentifier::Int ) {
        lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };
        errors->push_back( CCompilationError( expression->Location(), CCompilationError::INVALID_LENGTH_TYPE ) );
    } else {
        lastType = { CTypeIdentifier( TTypeIdentifier::IntArray ) };
    }

    lastType = { CTypeIdentifier( TTypeIdentifier::IntArray ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName, expression->Location() );

    std::string className = expression->TargetId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
        errors->push_back( CCompilationError( expression->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    lastType = { CTypeIdentifier( className ) };

    onNodeExit( nodeName, expression->Location() );
}

void CTypeCheckerVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName, expression->Location() );

    expression->TargetExpression()->Accept( this );
    // It should stay intact.
    lastType = { CTypeIdentifier( TTypeIdentifier::Boolean ) };

    onNodeExit( nodeName, expression->Location() );
}

/*__________ Statements __________*/

void CTypeCheckerVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName, statement->Location() );

    statement->LeftPart()->Accept( this );
    TTypeIdentifier leftPartLocalType = lastType.front().Type();

    statement->RightPart()->Accept( this );
    TTypeIdentifier rightPartLocalType = lastType.front().Type();

    if ( leftPartLocalType != rightPartLocalType ) {
        errors->push_back( CCompilationError( ( statement->LeftPart() )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, statement->Location() );
}

void CTypeCheckerVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName, statement->Location() );

    statement->RightPart()->Accept( this );
    TTypeIdentifier rightOperandType = lastType.front().Type();
    statement->LeftPartIndex()->Accept( this );
    TTypeIdentifier leftOperandType = lastType.front().Type();
    if ( leftOperandType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement->LeftPartId() )->Location(), CCompilationError::INVALID_INDEX_TYPE ) );
    }
    if ( rightOperandType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement->RightPart() )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
    }

    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, statement->Location() );
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_PRINT );
    onNodeEnter( nodeName, statement->Location() );

    statement->PrintTarget()->Accept( this );

    if ( lastType.front().Type() != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_PRINT_TYPE ) );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, statement->Location() );
}

void CTypeCheckerVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName, statement->Location() );

    statement->Condition()->Accept( this );
    if ( lastType.front().Type() != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }
    statement->PositiveTarget()->Accept( this );
    statement->NegativeTarget()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, statement->Location() );
}

void CTypeCheckerVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName, statement->Location() );

    statement->Condition()->Accept( this );
    if ( lastType.front().Type() != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }
    statement->Body()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, statement->Location() );
}

void CTypeCheckerVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_BRACES );
    onNodeEnter( nodeName, statement->Location() );

    statement->List()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, statement->Location() );
}

/*__________ Type Modifiers __________*/

// ignored
void CTypeCheckerVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName, typeModifier->Location() );
    onNodeExit( nodeName, typeModifier->Location() );
}

// ignored
void CTypeCheckerVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName, typeModifier->Location() );
    onNodeExit( nodeName, typeModifier->Location() );
}

// ignored
void CTypeCheckerVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName, typeModifier->Location() );
    onNodeExit( nodeName, typeModifier->Location() );
}

void CTypeCheckerVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName, typeModifier->Location() );

    std::string className = typeModifier->TypeId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
        errors->push_back( CCompilationError( typeModifier->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    lastType = { CTypeIdentifier( className ) };

    onNodeExit( nodeName, typeModifier->Location() );
}

/*__________ Other (except lists) __________*/

void CTypeCheckerVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    declaration->Type()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, declaration->Location() );
}

void CTypeCheckerVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG );
    onNodeEnter( nodeName, argument->Location() );

    argument->Type()->Accept( this );
    // Let lastType fall through.

    onNodeExit( nodeName, argument->Location() );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    std::string name = declaration->MethodId()->Name();
    lastMethod = lastClass->GetMethodDefinition( name );

    declaration->TypeModifier()->Accept( this );
    declaration->MethodArguments()->Accept( this );
    declaration->VarDeclarations()->Accept( this );
    declaration->Statements()->Accept( this );
    declaration->ReturnExpression()->Accept( this );

    lastMethod = nullptr;
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, declaration->Location() );
}

void CTypeCheckerVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName, mainClass->Location() );
    
    mainClass->Statements()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, mainClass->Location() );
}

void CTypeCheckerVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    using TClassDefinition = std::shared_ptr<const CClassDefinition>;

    std::string thisClassName = declaration->ClassName()->Name();
    lastClass = symbolTablePtr->GetClassDefinition(thisClassName);

    declaration->VarDeclarations()->Accept( this );
    declaration->MethodDeclarations()->Accept( this );

    if ( declaration->HasParent() ) {
        std::string parentName = declaration->ExtendsClassName()->Name();
        TClassDefinition parentClassDefinition = symbolTablePtr->GetClassDefinition(parentName);

        if ( parentClassDefinition == nullptr ) {
            errors->push_back( CCompilationError( declaration->Location(), CCompilationError::PARENT_CLASS_NOT_EXISTS ) );
        } else {
            while ( parentClassDefinition != nullptr && parentClassDefinition->HasParent() && parentClassDefinition->ClassName() != thisClassName ) {
                parentName = parentClassDefinition->GetParentName();
                parentClassDefinition = symbolTablePtr->GetClassDefinition(parentName);
            }

            if ( parentName == thisClassName ) {
                errors->push_back( CCompilationError( declaration->Location(), CCompilationError::CYCLIC_INHERITANCE ) );
            }
        }
    }

    lastClass = nullptr;
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, declaration->Location() );
}

void CTypeCheckerVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CNodeNames::PROGRAM );
    onNodeEnter( nodeName, program->Location() );

    program->MainClass()->Accept( this );
    program->ClassDeclarations()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, program->Location() );
}

/*__________  Lists __________*/

void CTypeCheckerVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName, list->Location() );

    std::vector<CTypeIdentifier> types;

    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );
        types.push_back(lastType.front());
    }

    lastType = types;

    onNodeExit( nodeName, list->Location() );
}

void CTypeCheckerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    for ( auto it = statements.rbegin(); it != statements.rend(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, list->Location() );
}

void CTypeCheckerVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {        
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, list->Location() );
}

void CTypeCheckerVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, list->Location() );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );   
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, list->Location() );
}

void CTypeCheckerVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName, list->Location() );
}
