#include <TypeCheckerVisitor.h>

std::shared_ptr<const std::vector<CCompilationError>> CTypeCheckerVisitor::Errors() const {
    return errors;
}

/*__________ Access Modifiers __________*/

void CTypeCheckerVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CTypeCheckerVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept( this );
    TTypeIdentifier leftOperandType = lastType;

    expression->RightOperand()->Accept( this );
    TTypeIdentifier RightOperandType = lastType;

    if ( leftOperandType != RightOperandType ) {
        errors->push_back( CCompilationError( ( expression )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
        lastType = TTypeIdentifier::NotFound;
    } else {
        lastType = leftOperandType;
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName );

    // write your code here

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

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CTypeCheckerVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    statement->LeftPart()->Accept( this );
    TTypeIdentifier leftPartLocalType = lastType;

    statement->RightPart()->Accept( this );
    TTypeIdentifier rightPartLocalType = lastType;

    if ( leftPartLocalType != rightPartLocalType ) {
        errors->push_back( CCompilationError( ( statement->LeftPart() )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
        lastType = TTypeIdentifier::NotFound;
    } else {
        lastType = leftPartLocalType;
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    statement->LeftPartId()->Accept( this );
    if ( lastType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement->LeftPartId() )->Location(), CCompilationError::INVALID_INDEX_TYPE ) );
    }

    // how to get type of elements in array???
    /*statement->LeftPart()->Accept( this );
    TTypeIdentifier leftPartLocalType = lastType;


    statement->RightPart()->Accept( this );
    TTypeIdentifier rightPartLocalType = lastType;

    if ( leftPartLocalType != rightPartLocalType ) {
        errors->push_back( CCompilationError( ( statement->LeftPart() )->Location(), CCompilationError::DIFFERENT_TYPES_OF_ARGUMENTS ) );
        lastType = TTypeIdentifier::NotFound;
    } else {
        lastType = leftPartLocalType;
    }
    */

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    statement->PrintTarget()->Accept( this );

    if ( lastType != TTypeIdentifier::Int ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_PRINT_TYPE ) );
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    if ( lastType != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    statement->Condition()->Accept( this );
    if ( lastType != TTypeIdentifier::Boolean ) {
        errors->push_back( CCompilationError( ( statement )->Location(), CCompilationError::INVALID_CONDITION_TYPE ) );
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    statement->List()->Accept( this );

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CTypeCheckerVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    std::string className = typeModifier->TypeId()->Name();

    if ( symbolTablePtr->GetClassDefinition(className) == nullptr ) {
    	errors->push_back( CCompilationError( typeModifier->Location(), CCompilationError::TYPE_NOT_EXISTS ) );
    }

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CTypeCheckerVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    declaration->Type()->Accept( this );

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG );
    onNodeEnter( nodeName );

    argument->Type()->Accept( this );

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

	declaration->TypeModifier()->Accept( this );
	declaration->MethodArguments()->Accept( this );
    declaration->VarDeclarations()->Accept( this );
    declaration->Statements()->Accept( this );

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL );
    onNodeEnter( nodeName );

    using TClassDefinition = std::shared_ptr<const CClassDefinition>;

    std::string thisClassName = declaration->ClassName()->Name();

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

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    program->ClassDeclarations()->Accept( this );

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

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    for ( auto it = statements.rbegin(); it != statements.rend(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {        
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );   
    }

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}
