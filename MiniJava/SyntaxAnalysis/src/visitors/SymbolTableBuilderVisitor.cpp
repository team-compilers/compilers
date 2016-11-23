#include <SymbolTableBuilderVisitor.h>

std::shared_ptr<const CSymbolTable> CSymbolTableBuilderVisitor::SymbolTable() const {
    return table;
}

std::shared_ptr<const std::vector<CCompilationError>> CSymbolTableBuilderVisitor::Errors() const {
    return errors;
}

/*__________ Access Modifiers __________*/

void CSymbolTableBuilderVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );

    lastAccessModifier = TAccessModifier::Public;

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );

    lastAccessModifier = TAccessModifier::Private;

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

// ignored
void CSymbolTableBuilderVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_ID );
    onNodeEnter( nodeName );

    lastId = expression->Name();

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

// ignored
void CSymbolTableBuilderVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CSymbolTableBuilderVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );

    lastType = CTypeIdentifier( TTypeIdentifier::Int );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    lastType = CTypeIdentifier( TTypeIdentifier::Boolean );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    lastType = CTypeIdentifier( TTypeIdentifier::IntArray );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    lastType = CTypeIdentifier( typeModifier->TypeId()->Name() );

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CSymbolTableBuilderVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    declaration->Type()->Accept( this );
    declaration->Id()->Accept( this );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG );
    onNodeEnter( nodeName );

    argument->Type()->Accept( this );
    argument->Id()->Accept( this );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

    declaration->AccessModifier()->Accept( this );
    TAccessModifier accessMod = lastAccessModifier;

    declaration->TypeModifier()->Accept( this );
    CTypeIdentifier returnType = lastType;

    declaration->MethodId()->Accept( this );
    std::string methodName = lastId;

    declaration->MethodArguments()->Accept( this );
    std::shared_ptr<VarNameToTypeMap> arguments = localVariableTypes;

    declaration->VarDeclarations()->Accept( this );
    lastMethodDefinition = std::make_shared<CMethodDefinition>( lastAccessModifier, methodName, returnType, arguments, localVariableTypes );
    
    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL );
    onNodeEnter( nodeName );

    declaration->ClassName()->Accept( this );
    std::string className = lastId;

    declaration->VarDeclarations()->Accept( this );
    std::shared_ptr<VarNameToTypeMap> fields = localVariableTypes;

    declaration->MethodDeclarations()->Accept( this );

    if ( declaration->HasParent() ) {
        declaration->ExtendsClassName()->Accept( this );
        lastClassDefinition = std::make_shared<CClassDefinition>( className, lastId, methodDefinitions, fields );
    } else {
        lastClassDefinition = std::make_shared<CClassDefinition>( className, methodDefinitions, fields );
    }
    
    methodDefinitions = nullptr;

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    program->ClassDeclarations()->Accept( this );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/
// ignored
void CSymbolTableBuilderVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    localVariableTypes = std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() );
    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        auto res = localVariableTypes->insert( std::make_pair( lastId, lastType ) );
        if ( !res.second ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_LOCAL_VAR ) );
        }
    }

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    localVariableTypes = std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() );
    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
        auto res = localVariableTypes->insert( std::make_pair( lastId, lastType ) );
        if ( !res.second ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_LOCAL_VAR ) );
        }
    }

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    methodDefinitions = std::shared_ptr<MethodNameToDefinitionMap>( new MethodNameToDefinitionMap() );
    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        auto res = methodDefinitions->insert(
            std::make_pair( lastMethodDefinition->MethodName(), lastMethodDefinition )
        );
        if ( !res.second ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_METHOD ) );
        }
    }

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        bool isAdded = table->AddClassDefinition( lastClassDefinition->ClassName(), lastClassDefinition );
        if ( !isAdded ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_CLASS ) );
        }
    }

    onNodeExit( nodeName );
}
