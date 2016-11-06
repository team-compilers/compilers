#include <PrintVisitor.h>

int CPrintVisitor::generateNodeNextIndex( const std::string& nodeType ) {
    int result = 0;
    auto insertResult = nodeTypeLastUsedIndex.insert( std::make_pair( nodeType, result ) );
    if ( !insertResult.second ) {
        result = ++insertResult.first->second;
    }
    return result;
}

std::string CPrintVisitor::generateNodeName( const std::string& nodeTypeName ) {
    return nodeTypeName + std::to_string( generateNodeNextIndex( nodeTypeName ) );
}

void CPrintVisitor::addEdge( const std::string& nodeFromName, const std::string& nodeToName ) {
    treeEdges[nodeFromName].push_back( nodeToName );
}

std::string CPrintVisitor::toString( const TOperandType& type ) const {
    std::string result;
    switch ( type ) {
        case TOperandType::OT_Plus: result = "Plus"; break;
        case TOperandType::OT_Minus: result = "Minus"; break;
        case TOperandType::OT_Times: result = "Times"; break;
        case TOperandType::OT_Div: result = "Div"; break;
        case TOperandType::OT_Mod: result = "Mod"; break;
        case TOperandType::OT_LT: result = "Less"; break;
        case TOperandType::OT_And: result = "And"; break;
        case TOperandType::OT_Or: result = "Or"; break;
        case TOperandType::OT_Count: result = "Count"; break;
    }
    return result;
}

void CPrintVisitor::onNodeEnter( const std::string& nodeName ) {
    if ( verbose ) {
        std::cout << margin << nodeEnterMarker << nodeName << std::endl;
        // margin.push_back( '\t' );
        margin.append( "  " );
    }
}

void CPrintVisitor::onNodeExit( const std::string& nodeName ) {
    if ( verbose ) {
        margin.erase( margin.size() - 2 );
        // margin.pop_back();
        std::cout << margin << nodeExitMarker << nodeName << std::endl;
    }
}

std::string CPrintVisitor::GetTraversalInDotLanguage() const {
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

void CPrintVisitor::Clear() {
    treeEdges.clear();
    nodeTypeLastUsedIndex.clear();
    visitedNodeStack.clear();
}

/*__________ Access Modifiers __________*/

void CPrintVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "AccessModPublic" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "AccessModPrivate" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CPrintVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpBinary" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName,  generateNodeName( toString( expression->Operation() ) ) );

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CBracketExpression* expression ) {
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

void CPrintVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNumber" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Value() ) + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpLogic" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::string( expression->Value() ? "true" : "false" ) + '\"';
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Id" ) + ": " + expression->Name() + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpLength" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LengthTarget()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CMethodExpression* expression ) {
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

void CPrintVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpThis" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNewArray" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LengthExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNewId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->TargetId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNegate" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->TargetExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CPrintVisitor::Visit( const CAssignIdStatement* statement ) {
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

void CPrintVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
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

void CPrintVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( "StatPrint" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->PrintTarget()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CConditionalStatement* statement ) {
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

void CPrintVisitor::Visit( const CWhileLoopStatement* statement ) {
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

void CPrintVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( "StatBraces" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->List()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CPrintVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModInt" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModBool" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModIntArray" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModId" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    typeModifier->TypeId()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CPrintVisitor::Visit( const CVarDeclaration* declaration ) {
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

void CPrintVisitor::Visit( const CMethodArgument* argument ) {
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

void CPrintVisitor::Visit( const CMethodDeclaration* declaration ) {
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

void CPrintVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( "MainClass" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    mainClass->ClassName()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    mainClass->ClassArgsName()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    mainClass->Statement()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CClassDeclaration* declaration ) {
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

void CPrintVisitor::Visit( const CProgram* program ) {
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

void CPrintVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( "ExpList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const IExpression> >& expressions = list->Expressions();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( "StatList" );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector< std::unique_ptr<const IStatement> >& statements = list->Statements();
    // must be reversed before being used
    for ( auto rit = statements.rbegin(); rit != statements.rend(); ++rit ) {
        ( *rit )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CPrintVisitor::Visit( const CVarDeclarationList* list ) {
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

void CPrintVisitor::Visit( const CMethodArgumentList* list ) {
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

void CPrintVisitor::Visit( const CMethodDeclarationList* list ) {
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

void CPrintVisitor::Visit( const CClassDeclarationList* list ) {
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
