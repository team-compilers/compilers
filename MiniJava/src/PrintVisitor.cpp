#include <PrintVisitor.h>
#include <Expression.h>
#include <Statement.h>


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


void CPrintVisitor::Visit( CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( "OpExp" );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName,  generateNodeName( toString( expression->Operation ) ) );

    expression->RightOperand->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "IdExp" );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = generateNodeName( "Id" ) + ": " + expression->Name;
    addEdge( nodeName, valueNodeName );
}


void CPrintVisitor::Visit( CBracketExpression* expression ) {

}

void CPrintVisitor::Visit( CNumberExpression* expression ) {
    
}

void CPrintVisitor::Visit( CLogicExpression* expression ) {
    
}

void CPrintVisitor::Visit( CLengthExpression* expression ) {
    
}

void CPrintVisitor::Visit( CMethodExpression* expression ) {
    
}

void CPrintVisitor::Visit( CThisExpression* expression ) {
    
}

void CPrintVisitor::Visit( CNewArrayExpression* expression ) {
    
}

void CPrintVisitor::Visit( CNewIdExpression* expression ) {
    
}

void CPrintVisitor::Visit( CNegateExpression* expression ) {
    
}

void CPrintVisitor::Visit( CAssignIdStatement* statement ) {
    
}

void CPrintVisitor::Visit( CAssignIdWithIndexStatement* statement ) {
    
}

void CPrintVisitor::Visit( CPrintStatement* statement ) {
    
}

void CPrintVisitor::Visit( CConditionalStatement* statement ) {
}

void CPrintVisitor::Visit( CWhileLoopStatement* statement ) {
    
}

void CPrintVisitor::Visit( CBracesStatement* statement ) {
    
}

void CPrintVisitor::Visit( CPrivateAccessModifier* modifier ) {
    
}

void CPrintVisitor::Visit( CPublicAccessModifier* modifier ) {
    
}

void CPrintVisitor::Visit( CMethodArgument* argument ) {
    
}

void CPrintVisitor::Visit( CMethodDeclaration* declaration ) {
    
}

void CPrintVisitor::Visit( CIntTypeModifier* intModifier ) {
    
}

void CPrintVisitor::Visit( CBooleanTypeModifier* booleanModifier ) {
    
}

void CPrintVisitor::Visit( CIntArrayTypeModifier* intArrayModifier ) {
    
}

void CPrintVisitor::Visit( CIdTypeModifier* idModifier ) {
    
}

void CPrintVisitor::Visit( CClassDeclaration* classDeclaration ) {
    
}

void CPrintVisitor::Visit( CMainClass* mainClass ) {
    
}

void CPrintVisitor::Visit( CVarDeclaration* varDeclaration ) {
    
}

void CPrintVisitor::Visit( CProgram* program ) {
    
}

void CPrintVisitor::Visit( CStatementList* list ) {
    
}

void CPrintVisitor::Visit( CExpressionList* list ) {
    
}

void CPrintVisitor::Visit( CMethodArgumentList* list ) {
    
}

void CPrintVisitor::Visit( CMethodDeclarationList* list ) {
    
}

void CPrintVisitor::Visit( CClassDeclarationList* list ) {
    
}

void CPrintVisitor::Visit( CVarDeclarationList* list ) {
    
}
