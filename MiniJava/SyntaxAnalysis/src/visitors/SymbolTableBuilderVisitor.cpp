#include <SymbolTableBuilderVisitor.h>

const CSymbolTableBuilderVisitor::CSymbolTable& SymbolTable() const {
    return table;
}

const std::vector<CCompilationError> CSymbolTableBuilderVisitor::Errors() const {
    return errors;
}

/*__________ Access Modifiers __________*/

void CSymbolTableBuilderVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );

    accessModLast = TAccessModifier::Public;

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );

    accessModLast = TAccessModifier::Private;

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

    idLast = expression->Name();

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

    typeLast = CTypeIdentifier( TTypeIdentifier::Int );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    typeLast = CTypeIdentifier( TTypeIdentifier::Boolean );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    typeLast = CTypeIdentifier( TTypeIdentifier::IntArray );

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    typeLast = CTypeIdentifier( typeModifier->TypeId()->Name() );

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
    TAccessModifier accessMod = accessModLast;

    declaration->TypeModifier()->Accept( this );
    CTypeIdentifier returnType = typeLast;

    declaration->MethodId()->Accept( this );
    const std::string& methodName = idLast;

    declaration->MethodArguments()->Accept( this );
    std::unordered_map<std::string, CTypeIdentifier> arguments = localVariableTypes;

    declaration->VarDeclarations()->Accept( this );

    methodDefinitionLast = CMethodDefinition( accessModLast, methodName, returnType, arguments, localVariableTypes);

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
    const std::string& className = idLast;

    declaration->VarDeclarations()->Accept( this );
    std::unordered_map<std::string, CTypeIdentifier> fields = localVariableTypes;

    declaration->MethodDeclarations()->Accept( this );

    if ( declaration->HasParent() ) {
        declaration->ExtendsClassName()->Accept( this );
        const std::string& parentName = idLast;
        classDefinitionLast = CClassDefinition( className, parentName, methodDefinitions, fields );
    } else {
        classDefinitionLast = CClassDefinition( className, methodDefinitions, fields );
    }

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

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        auto res = localVariableTypes.insert( std::make_pair<std::string, CTypeIdentifier>( idLast, typeLast ) );
        if ( !res.second ) {
            errors.push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_LOCAL_VAR ) );
        }
    }

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
        auto res = localVariableTypes.insert( std::make_pair<std::string, CTypeIdentifier>( idLast, typeLast ) );
        if ( !res.second ) {
            errors.push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_LOCAL_VAR ) );
        }
    }

    onNodeExit( nodeName );
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        auto res = methodDefinitions.insert(
            std::make_pair<std::string, std::unique_ptr<CMethodDefinition>>(
                methodDefinitionLast.MethodName(),
                std::make_unique< CMethodDefinition >( methodDefinitionLast )
            );
        );
        if ( !res.second ) {
            errors.push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_METHOD ) );
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
        bool isAdded = table.AddClassDefinition( classDefinitionLast.ClassName(), classDefinitionLast );
        if ( !isAdded ) {
            errors.push_back( CCompilationError( ( *it )->Location(), CCompilationError::REDEFINITION_CLASS ) );
        }
    }

    onNodeExit( nodeName );
}
