#include <AST/visitors/SymbolTableBuilderVisitor.h>

using namespace ASTree;

const CSymbolTable* CSymbolTableBuilderVisitor::SymbolTable() const {
    return table;
}

const std::vector<CCompilationError>* CSymbolTableBuilderVisitor::Errors() const {
    return errors;
}

/*__________ Access Modifiers __________*/

void CSymbolTableBuilderVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName, modifier->Location() );

    lastAccessModifier = TAccessModifier::Public;

    onNodeExit( nodeName, modifier->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName, modifier->Location() );

    lastAccessModifier = TAccessModifier::Private;

    onNodeExit( nodeName, modifier->Location() );
}

/*__________ Expressions __________*/

// ignored
void CSymbolTableBuilderVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ID );
    onNodeEnter( nodeName, expression->Location() );

    lastId.push_back( expression->Name() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_METHOD );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_THIS );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName, expression->Location() );

    onNodeExit( nodeName, expression->Location() );
}

/*__________ Statements __________*/

// ignored
void CSymbolTableBuilderVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName, statement->Location() );

    onNodeExit( nodeName, statement->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName, statement->Location() );

    onNodeExit( nodeName, statement->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_PRINT );
    onNodeEnter( nodeName, statement->Location() );

    onNodeExit( nodeName, statement->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName, statement->Location() );

    onNodeExit( nodeName, statement->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName, statement->Location() );

    onNodeExit( nodeName, statement->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_BRACES );
    onNodeEnter( nodeName, statement->Location() );

    onNodeExit( nodeName, statement->Location() );
}

/*__________ Type Modifiers __________*/

void CSymbolTableBuilderVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName, typeModifier->Location() );

    lastType = CTypeIdentifier( TTypeIdentifier::Int );

    onNodeExit( nodeName, typeModifier->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName, typeModifier->Location() );

    lastType = CTypeIdentifier( TTypeIdentifier::Boolean );

    onNodeExit( nodeName, typeModifier->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName, typeModifier->Location() );

    lastType = CTypeIdentifier( TTypeIdentifier::IntArray );

    onNodeExit( nodeName, typeModifier->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName, typeModifier->Location() );

    lastType = CTypeIdentifier( typeModifier->TypeId()->Name() );

    onNodeExit( nodeName, typeModifier->Location() );
}

/*__________ Other (except lists) __________*/

void CSymbolTableBuilderVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    declaration->Type()->Accept( this ); // fills lastType
    declaration->Id()->Accept( this ); // fills lastId.back()

    onNodeExit( nodeName, declaration->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG );
    onNodeEnter( nodeName, argument->Location() );

    argument->Type()->Accept( this ); // fills lastType
    argument->Id()->Accept( this ); // fills lastId.back()

    onNodeExit( nodeName, argument->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    declaration->AccessModifier()->Accept( this ); // fills lastAccessModifier

    localVariableTypes.push_back( std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() ) );
    sortedArgumentsTypes = std::make_shared<ArgumentsTypesList>();

    declaration->MethodArguments()->Accept( this ); // fills localVariableTypes.back()

    localVariableTypes.push_back( std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() ) );
    declaration->VarDeclarations()->Accept( this ); // fills localVariableTypes.back()
    
    declaration->TypeModifier()->Accept( this ); // fills lastType

    declaration->MethodId()->Accept( this ); // fills lastId.back()

    lastMethodDefinition = std::make_shared<CMethodDefinition>( 
        lastAccessModifier, lastId.back(), lastType,
        localVariableTypes.at(localVariableTypes.size() - 2), 
        sortedArgumentsTypes,
        localVariableTypes.back()
    );

    sortedArgumentsTypes = nullptr;
    
    onNodeExit( nodeName, declaration->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName, mainClass->Location() );

    lastId.clear();
    mainClass->ClassName()->Accept( this ); // fills lastId[0]

    methodDefinitions = std::shared_ptr<MethodNameToDefinitionMap>( new MethodNameToDefinitionMap() );
    lastMethodDefinition = std::make_shared<CMethodDefinition>( 
        TAccessModifier::Public,
        "main",
        CTypeIdentifier( TTypeIdentifier::Int ), // we do not have `void`, let it be `int`
        std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() ), // no arguments
        std::make_shared<ArgumentsTypesList>(),
        std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() ) // no local variables
    );
    auto res = methodDefinitions->insert(
        std::make_pair( lastMethodDefinition->MethodName(), lastMethodDefinition )
    );
    if ( !res.second ) {
        errors->push_back( CCompilationError( mainClass->Location(), CCompilationError::REDEFINITION_METHOD ) );
    }

    std::shared_ptr<VarNameToTypeMap> localVariables( new VarNameToTypeMap() );
    lastClassDefinition = std::make_shared<CClassDefinition>( lastId[0], methodDefinitions, localVariables );

    onNodeExit( nodeName, mainClass->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    lastId.clear();
    declaration->ClassName()->Accept( this ); // fills lastId[0]

    localVariableTypes = std::vector<std::shared_ptr<VarNameToTypeMap>> {
        std::shared_ptr<VarNameToTypeMap>( new VarNameToTypeMap() )
    };
    declaration->VarDeclarations()->Accept( this ); // fills localVariableTypes[0]

    methodDefinitions = std::shared_ptr<MethodNameToDefinitionMap>( new MethodNameToDefinitionMap() );
    declaration->MethodDeclarations()->Accept( this ); // fills methodDefinitions

    if ( declaration->HasParent() ) {
        declaration->ExtendsClassName()->Accept( this ); // fills lastId.back()
        lastClassDefinition = std::make_shared<CClassDefinition>( lastId[0], lastId.back(), methodDefinitions, localVariableTypes[0] );
    } else {
        lastClassDefinition = std::make_shared<CClassDefinition>( lastId[0], methodDefinitions, localVariableTypes[0] );
    }
    
    onNodeExit( nodeName, declaration->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CNodeNames::PROGRAM );
    onNodeEnter( nodeName, program->Location() );

    program->MainClass()->Accept( this );
    bool isAdded = table->AddClassDefinition( lastClassDefinition->ClassName(), lastClassDefinition );
    if ( !isAdded ) {
        errors->push_back( CCompilationError( program->MainClass()->Location(), CCompilationError::REDEFINITION_CLASS ) );
    }

    program->ClassDeclarations()->Accept( this );

    onNodeExit( nodeName, program->Location() );
}

/*__________  Lists __________*/
// ignored
void CSymbolTableBuilderVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName, list->Location() );

    onNodeExit( nodeName, list->Location() );
}

// ignored
void CSymbolTableBuilderVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName, list->Location() );

    onNodeExit( nodeName, list->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {        
        ( *it )->Accept( this ); // fills lastId.back() and lastType
        auto res = localVariableTypes.back()->insert( std::make_pair( lastId.back(), lastType ) );
        if ( !res.second ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_LOCAL_VAR ) );
        }
    }

    onNodeExit( nodeName, list->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this ); // fills lastId.back() and lastType
        auto res = localVariableTypes.back()->insert( std::make_pair( lastId.back(), lastType ) );
        sortedArgumentsTypes->push_back(lastType);
        if ( !res.second ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_LOCAL_VAR ) );
        }
    }

    onNodeExit( nodeName, list->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

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

    onNodeExit( nodeName, list->Location() );
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        bool isAdded = table->AddClassDefinition( lastClassDefinition->ClassName(), lastClassDefinition );
        if ( !isAdded ) {
            errors->push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_CLASS ) );
        }
    }

    onNodeExit( nodeName, list->Location() );
}
