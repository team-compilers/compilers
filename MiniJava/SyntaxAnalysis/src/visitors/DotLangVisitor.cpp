#include <DotLangVisitor.h>

void CDotLangVisitor::addEdge( const std::string& nodeFromName, const std::string& nodeToName ) {
    treeEdges[nodeFromName].push_back( nodeToName );
}

std::string CDotLangVisitor::GetTraversalInDotLanguage() const {
    std::stringstream sstream;
    sstream << "digraph {" << std::endl;
    sstream << '\t' << "ordering = out;" << std::endl;
    for ( auto it = treeEdges.begin(); it != treeEdges.end(); ++it ) {
        const std::string& fromNode = it->first;
        for ( const std::string& toNode : it->second ) {
            sstream << '\t' << fromNode << " -> " << toNode << ';' << std::endl;
        }
    }
    sstream << '}' << std::endl;

    return sstream.str();
}

void CDotLangVisitor::Clear() {
    treeEdges.clear();
    nodeTypeLastUsedIndex.clear();
    visitedNodeStack.clear();
}

/*__________ Access Modifiers __________*/

void CDotLangVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "AccessModPublic" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "AccessModPrivate" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CDotLangVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpBinary" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName,  generateNodeName( operatorName( expression->Operation() ) ) );

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpBracket" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->ContainerExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->IndexExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNumber" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Value() ) + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpLogic" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::string( expression->Value() ? "true" : "false" ) + '\"';
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Id" ) + ": " + expression->Name() + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpLength" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LengthTarget()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpMethod" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->CallerExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->MethodId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->Arguments()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpThis" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNewArray" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LengthExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNewId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->TargetId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNegate" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->TargetExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CDotLangVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( "StatAssignId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->LeftPart()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->RightPart()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( "StatAssignIdWithIndex" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->LeftPartId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->LeftPartIndex()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->RightPart()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( "StatPrint" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->PrintTarget()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( "StatConditional" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->Condition()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->PositiveTarget()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->NegativeTarget()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( "StatWhileLoop" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->Condition()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->Body()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( "StatBraces" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->List()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CDotLangVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModInt" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModBool" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModIntArray" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    typeModifier->TypeId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CDotLangVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "VarDecl" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    declaration->Type()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->Id()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( "MethArg" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    argument->Type()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    argument->Id()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "MethDecl" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    declaration->AccessModifier()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->TypeModifier()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->MethodId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->MethodArguments()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->VarDeclarations()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->Statements()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->ReturnExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( "MainClass" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    mainClass->ClassName()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    mainClass->ClassArgsName()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    mainClass->Statements()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "ClassDecl" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    declaration->ClassName()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->VarDeclarations()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    declaration->MethodDeclarations()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    if ( declaration->HasParent() ) {
        declaration->ExtendsClassName()->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( "Program" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    program->MainClass()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    program->ClassDeclarations()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CDotLangVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( "ExpList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( "StatList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    // must be reversed before being used
    for ( auto rit = statements.rbegin(); rit != statements.rend(); ++rit ) {
        ( *rit )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( "VarDeclList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( "MethArgList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( "MethDeclList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( "ClassDeclList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}
