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
    switch (type) {
        case TOperandType::OT_Plus: result = "Plus"; break;
        case TOperandType::OT_Minus: result = "Minus"; break;
        case TOperandType::OT_Times: result = "Times"; break;
        case TOperandType::OT_Div: result = "Div"; break;
        case TOperandType::OT_Count: result = "Count"; break;
    }
    return result;
}

void CPrintVisitor::Visit( CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( "OpExp" );
    visitedNodeStack.push( nodeName );

    expression->LeftOperand->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();

    addEdge( nodeName,  generateNodeName( toString( expression->Operation ) ) );

    expression->RightOperand->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();
}

void CPrintVisitor::Visit( CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( "NumExp" );
    visitedNodeStack.push( nodeName );

    std::string valueNodeName = generateNodeName( "Value" ) + ": " + std::to_string( expression->Value );
    addEdge( nodeName, valueNodeName );
}

void CPrintVisitor::Visit( CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "IdExp" );
    visitedNodeStack.push( nodeName );

    std::string valueNodeName = generateNodeName( "Id" ) + ": " + expression->Name;
    addEdge( nodeName, valueNodeName );
}

void CPrintVisitor::Visit( CPairListExpression* expression ) {
    std::string nodeName = generateNodeName( "PairExpList" );
    visitedNodeStack.push( nodeName );

    expression->Head->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();

    expression->Tail->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();
}

void CPrintVisitor::Visit( CSingleElementListExpression* expression ) {
    std::string nodeName = generateNodeName( "LastExpList" );
    visitedNodeStack.push( nodeName );

    expression->Element->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();
}

void CPrintVisitor::Visit( CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( "PrintStm" );
    visitedNodeStack.push( nodeName );

    statement->PrintList->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();
}

void CPrintVisitor::Visit( CAssignStatement* statement ) {
    std::string nodeName = generateNodeName( "AssignStm" );
    visitedNodeStack.push( nodeName );

    statement->LeftValue->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();

    statement->RightValue->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();
}

void CPrintVisitor::Visit( CCompoundStatement* statement ) {
    std::string nodeName = generateNodeName( "CompoundStm" );
    visitedNodeStack.push( nodeName );

    statement->First->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();

    statement->Second->Accept( this );
    addEdge( nodeName, visitedNodeStack.top() );
    visitedNodeStack.pop();
}
