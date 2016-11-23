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

    // write your code here

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

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    // write your code here

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

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    // write your code here

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

    // write your code here

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CTypeCheckerVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

    // write your code here

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

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    // write your code here

    onNodeExit( nodeName );
}

void CTypeCheckerVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    // write your code here

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
