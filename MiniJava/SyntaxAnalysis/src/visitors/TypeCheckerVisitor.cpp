#include <TypeCheckerVisitor.h>

std::shared_ptr<const std::vector<CCompilationError>> CTypeCheckerVisitor::Errors() const {
    return errors;
}

std::shared_ptr<const CMethodDefinition> CTypeCheckerVisitor::searchClassHierarchyForMethod( const std::string& methodName, std::shared_ptr<const CClassDefinition> baseClass) {
    std::shared_ptr<const CMethodDefinition> methodDefinition = nullptr;
    while ( baseClass != nullptr ) {
        methodDefinition = baseClass->GetMethodDefinition( methodName );
        if ( methodDefinition != nullptr ) {
            break;
        }
        baseClass = baseClass->HasParent() ? symbolTablePtr->GetClassDefinition( baseClass->GetParentName() ) : nullptr;
    }
    return methodDefinition;
}

CTypeIdentifier CTypeCheckerVisitor::searchClassHierarchyForField( const std::string& fieldName, std::shared_ptr<const CClassDefinition> baseClass) {
    CTypeIdentifier fieldType( TTypeIdentifier::NotFound );
    while ( baseClass != nullptr ) {
        fieldType = baseClass->GetFieldType( fieldName );
        if ( fieldType.Type() != TTypeIdentifier::NotFound ) {
            break;
        }
        baseClass = baseClass->HasParent() ? symbolTablePtr->GetClassDefinition( baseClass->GetParentName() ) : nullptr;
    }
    return fieldType;
}

/*__________ Access Modifiers __________*/

// ignored
void CTypeCheckerVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );
    onNodeExit( nodeName );
}

// ignored
void CTypeCheckerVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );
    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CTypeCheckerVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

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
    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName );

    expression->ContainerExpression()->Accept( this );

    TTypeIdentifier containerType;
    if( lastType.front().Type() == TTypeIdentifier::IntArray ) {
        containerType = TTypeIdentifier::Int;
    } else {
        containerType = TTypeIdentifier::NotFound;
    }

    expression->IndexExpression()->Accept( this );

    if( lastType.front().Type() != TTypeIdentifier::Int ) {
    	errors->push_back( CCompilationError( ( expression )->Location(), CCompilationError::INVALID_INDEX_TYPE ) );
    }

    lastType = { CTypeIdentifier( containerType ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName );

    lastType = { CTypeIdentifier( TTypeIdentifier::Int ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    lastType = { CTypeIdentifier( TTypeIdentifier::Boolean ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_ID );
    onNodeEnter( nodeName );

    std::string name = expression->Name();
    CTypeIdentifier notFound(TTypeIdentifier::NotFound);

    CTypeIdentifier fieldLurk = searchClassHierarchyForField( name, lastClass );
    // TODO: do we really have to perform methodLurk?
    std::shared_ptr<const CMethodDefinition> methodLurk = searchClassHierarchyForMethod( name, lastClass );
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

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    expression->LengthTarget()->Accept ( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::Int ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    CTypeIdentifier methodReturnType = CTypeIdentifier( TTypeIdentifier::NotFound );
    expression->CallerExpression()->Accept( this );

    std::string methodName = expression->MethodId()->Name();
    std::string className = lastType.front().ClassName();

    std::shared_ptr<const CClassDefinition> callerClassDefinition;

    callerClassDefinition = symbolTablePtr->GetClassDefinition( className );

    if( callerClassDefinition == nullptr ) {
        errors->emplace_back( expression->Location(), CCompilationError::INVALID_CALLER_EXPRESSION );
    } else {
	    std::shared_ptr<const CMethodDefinition> methodDefinition =
	        callerClassDefinition->GetMethodDefinition( methodName );

	    if( methodDefinition == nullptr ) {
	        errors->emplace_back( expression->Location(), CCompilationError::CLASS_HAS_NO_METHOD );
	        methodReturnType = CTypeIdentifier( TTypeIdentifier::NotFound );
	    } else {
	    	if ( methodDefinition->AccessModifier() == TAccessModifier::Private &&
	    		 callerClassDefinition != lastClass ) {
	    		errors->emplace_back( expression->Location(), CCompilationError::METHOD_IS_PRIVATE );
	    	}

    		expression->Arguments()->Accept( this );
    		if( lastType.size() != methodDefinition->GetArgumentsNumber() ) {
    			errors->emplace_back( expression->Location(), CCompilationError::ARGS_NUMBERS_NOT_MATCH );
    		} else {
    			for( int i = 0; i < lastType.size(); ++i ) {
    				if (lastType[i] != methodDefinition->GetArgumentType(i) ) {
    					errors->emplace_back( expression->Location(), CCompilationError::ARG_TYPE_NOT_MATCH );
    				}
    			}
    		}
	    	methodReturnType = methodDefinition->ReturnType();
	    }
	}

    lastType = { methodReturnType };

    onNodeExit( nodeName );
}


// ignored
void CTypeCheckerVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    lastType = { CTypeIdentifier( lastClass->ClassName() ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    expression->LengthExpression()->Accept( this );

    if ( lastType.front().Type() != TTypeIdentifier::Int ) {
        lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };
        errors->push_back( CCompilationError( expression->Location(), CCompilationError::INVALID_LENGTH_TYPE ) );
    } else {
        lastType = { CTypeIdentifier( TTypeIdentifier::IntArray ) };
    }

    lastType = { CTypeIdentifier( TTypeIdentifier::IntArray ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    std::string className = expression->TargetId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
        errors->push_back( CCompilationError( expression->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    lastType = { CTypeIdentifier( className ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    expression->TargetExpression()->Accept( this );
    // It should stay intact.
    lastType = { CTypeIdentifier( TTypeIdentifier::Boolean ) };

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CTypeCheckerVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    statement->LeftPart()->Accept( this );
    TTypeIdentifier leftPartLocalType = lastType.front().Type();

    statement->RightPart()->Accept( this );
    TTypeIdentifier rightPartLocalType = lastType.front().Type();

    if ( leftPartLocalType != rightPartLocalType ) {
        errors->push_back( CCompilationError( ( statement->LeftPart() )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

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

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    statement->PrintTarget()->Accept( this );

    if ( lastType.front().Type() != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_PRINT_TYPE ) );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    if ( lastType.front().Type() != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }
    statement->PositiveTarget()->Accept( this );
    statement->NegativeTarget()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    if ( lastType.front().Type() != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }
    statement->Body()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    statement->List()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

// ignored
void CTypeCheckerVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );
    onNodeExit( nodeName );
}

// ignored
void CTypeCheckerVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );
    onNodeExit( nodeName );
}

// ignored
void CTypeCheckerVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );
    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    std::string className = typeModifier->TypeId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
        errors->push_back( CCompilationError( typeModifier->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    lastType = { CTypeIdentifier( className ) };

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CTypeCheckerVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    declaration->Type()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG );
    onNodeEnter( nodeName );

    argument->Type()->Accept( this );
    // Let lastType fall through.

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

    std::string name = declaration->MethodId()->Name();
    lastMethod = lastClass->GetMethodDefinition(name);
    std::shared_ptr<const CClassDefinition> precedingClass = lastClass;
    while (precedingClass->HasParent()) {
        precedingClass = symbolTablePtr->GetClassDefinition(precedingClass->GetParentName());
        if (precedingClass == nullptr) {
            break;
        } else if (precedingClass->GetMethodDefinition(name) != nullptr) {
            errors->push_back( CCompilationError( declaration->Location(), CCompilationError::METHOD_OVERLOADING ) );
            break;
        }
    }

    declaration->TypeModifier()->Accept( this );
    declaration->MethodArguments()->Accept( this );
    declaration->VarDeclarations()->Accept( this );
    declaration->Statements()->Accept( this );
    declaration->ReturnExpression()->Accept( this );

    lastMethod = nullptr;
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );
    
    mainClass->Statements()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL );
    onNodeEnter( nodeName );

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

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    program->MainClass()->Accept( this );
    program->ClassDeclarations()->Accept( this );
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CTypeCheckerVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    std::vector<CTypeIdentifier> types;

    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    for ( auto it = expressions.rbegin(); it != expressions.rend(); ++it ) {
        ( *it )->Accept( this );
        types.push_back(lastType.front());
    }

    lastType = types;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    for ( auto it = statements.rbegin(); it != statements.rend(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {        
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );   
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = { CTypeIdentifier( TTypeIdentifier::NotFound ) };

    onNodeExit( nodeName );
}
