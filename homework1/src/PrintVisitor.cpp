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

void CPrintVisitor::Visit( CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( "NumExp" );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = generateNodeName( "Value" ) + ": " + std::to_string( expression->Value );
    addEdge( nodeName, valueNodeName );
}

void CPrintVisitor::Visit( CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "IdExp" );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = generateNodeName( "Id" ) + ": " + expression->Name;
    addEdge( nodeName, valueNodeName );
}

void CPrintVisitor::Visit( CPairListExpression* expression ) {
    std::string nodeName = generateNodeName( "PairExpList" );
    visitedNodeStack.push_back( nodeName );

    expression->Head->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->Tail->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CSingleElementListExpression* expression ) {
    std::string nodeName = generateNodeName( "LastExpList" );
    visitedNodeStack.push_back( nodeName );

    expression->Element->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( "PrintStm" );
    visitedNodeStack.push_back( nodeName );

    statement->PrintList->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CAssignStatement* statement ) {
    std::string nodeName = generateNodeName( "AssignStm" );
    visitedNodeStack.push_back( nodeName );

    statement->LeftValue->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->RightValue->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CPrintVisitor::Visit( CCompoundStatement* statement ) {
    std::string nodeName = generateNodeName( "CompoundStm" );
    visitedNodeStack.push_back( nodeName );

    statement->First->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->Second->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}
