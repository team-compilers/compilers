#include <TypeCheckerVisitor.h>

std::shared_ptr<const std::vector<CCompilationError>> CTypeCheckerVisitor::Errors() const {
    return errors;
}

static void printType( TTypeIdentifier type )
{
    std::string string;
    switch( type )
    {
        case TTypeIdentifier::Int:
        { 
            string = "int";
            break;
        }
        case TTypeIdentifier::Boolean:
        {
            string = "boolean";
            break;
        }
        case TTypeIdentifier::NotFound: 
        {
            string = "NotFound";
            break;
        }
        case TTypeIdentifier::ClassId:
        {
        	string = "classId";
        	break;
        }
        default: {
        	string = "other";
        	break;
        }
    }
    std::cout << string << std::endl;
}

/*__________ Access Modifiers __________*/

void CTypeCheckerVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );

    // TODO

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );

    // TODO

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CTypeCheckerVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    TTypeIdentifier operatorType; 
    if ( expression->Operation() == TOperatorType::OT_LT ) {
        operatorType = TTypeIdentifier::Boolean;
    } else {
        operatorType = TTypeIdentifier::Int;
    }
    expression->LeftOperand()->Accept( this );
    TTypeIdentifier leftOperandType = lastType;

    expression->RightOperand()->Accept( this );
    TTypeIdentifier RightOperandType = lastType;

    if ( leftOperandType != RightOperandType ) {
        errors->push_back( CCompilationError( ( expression )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
        lastType = TTypeIdentifier::NotFound;
    } else {
        lastType = operatorType;
    }
    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName );

    expression->ContainerExpression()->Accept( this );

    TTypeIdentifier containerType;
    if(lastType == TTypeIdentifier::IntArray) {
    	containerType = TTypeIdentifier::Int;
    } else {
    	containerType = TTypeIdentifier::NotFound;
    }

    expression->IndexExpression()->Accept( this );

    lastType = containerType;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName );

    lastType = TTypeIdentifier::Int;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    lastType = TTypeIdentifier::Boolean;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_ID );
    onNodeEnter( nodeName );

    std::string name = expression->Name();
    CTypeIdentifier notFound(TTypeIdentifier::NotFound);

    CTypeIdentifier fieldLurk = lastClass->GetFieldType(name);
    CTypeIdentifier localLurk = lastMethod->GetLocalVariableType(name);
    CTypeIdentifier argumentLurk = lastMethod->GetArgumentType(name);
    //printType(fieldLurk.Type());
    //printType(localLurk.Type());
    //printType(argumentLurk.Type());
    lastType = TTypeIdentifier::NotFound;

    // The latter wins.
    if (fieldLurk != notFound) {
    	lastType = fieldLurk.Type();
    }
    if (argumentLurk != notFound) {
    	lastType = argumentLurk.Type();
    }
    if (localLurk != notFound) {
    	lastType = localLurk.Type();
    }

    if (lastType == TTypeIdentifier::NotFound) {
    	errors->push_back( CCompilationError( expression->Location(), CCompilationError::VAR_UNDEFINED ) );
    }
    //printType(lastType);
    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    expression->LengthTarget()->Accept ( this );

    lastType = TTypeIdentifier::Int;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    expression->CallerExpression()->Accept( this );

    // TODO: lastType

    expression->Arguments()->Accept( this );

    onNodeExit( nodeName );
}


// ignored
void CTypeCheckerVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    lastType = TTypeIdentifier::ClassId;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    expression->LengthExpression()->Accept( this );

    if ( lastType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( expression->Location(), CCompilationError::INVALID_LENGTH_TYPE ) );
    }

    lastType = TTypeIdentifier::IntArray;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    std::string className = expression->TargetId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
    	errors->push_back( CCompilationError( expression->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    lastType = TTypeIdentifier::ClassId;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    expression->TargetExpression()->Accept( this );
    // It should stay intact.
    lastType = TTypeIdentifier::Boolean;

    onNodeExit( nodeName );
}

/*__________ Statements __________*/
// Statements do not have type, the fuck are you doing with lastType?

void CTypeCheckerVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    statement->LeftPart()->Accept( this );
    TTypeIdentifier leftPartLocalType = lastType;

    statement->RightPart()->Accept( this );
    TTypeIdentifier rightPartLocalType = lastType;

    if ( leftPartLocalType != rightPartLocalType ) {
        errors->push_back( CCompilationError( ( statement->LeftPart() )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    statement->LeftPartId()->Accept( this );
    if ( lastType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement->LeftPartId() )->Location(), CCompilationError::INVALID_INDEX_TYPE ) );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    statement->PrintTarget()->Accept( this );

    if ( lastType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_PRINT_TYPE ) );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    if ( lastType != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    if ( lastType != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    statement->List()->Accept( this );
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CTypeCheckerVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );

    // TODO

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    // TODO

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    // TODO

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    std::string className = typeModifier->TypeId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
    	errors->push_back( CCompilationError( typeModifier->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    lastType = TTypeIdentifier::ClassId;

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CTypeCheckerVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    declaration->Type()->Accept( this );
    lastType = TTypeIdentifier::NotFound;

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
	lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

// ignored
void CTypeCheckerVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );
    
	lastType = TTypeIdentifier::NotFound;

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
	lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    program->ClassDeclarations()->Accept( this );
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CTypeCheckerVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    for ( auto it = expressions.rbegin(); it != expressions.rend(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    for ( auto it = statements.rbegin(); it != statements.rend(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {        
        ( *it )->Accept( this );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );   
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }
    lastType = TTypeIdentifier::NotFound;

    onNodeExit( nodeName );
}
