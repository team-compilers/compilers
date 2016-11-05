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

void CPrintVisitor::Visit( CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "AccessModPublic" );
}

void CPrintVisitor::Visit( CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( "BinExp" );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName,  generateNodeName( toString( expression->Operation() ) ) );

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( "BracketExp" );
    visitedNodeStack.push_back( nodeName );

    expression->ContainerExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName, "[]" );

    expression->IndexExpression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( "NumberExp" ); 
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = generateNodeName("Value") + ": " + std::to_string( expression->Value() );
    addEdge( nodeName, valueNodeName );
}

void CPrintVisitor::Visit( CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "IdExp" );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = generateNodeName( "Id" ) + ": " + expression->Name();
    addEdge( nodeName, valueNodeName );
}

void CPrintVisitor::Visit( CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CThisExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CProgram* program ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CExpressionList* list ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CStatementList* list ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}

void CPrintVisitor::Visit( CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( "" );
    visitedNodeStack.push_back( nodeName );
}
