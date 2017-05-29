#include <Synthesis/visitors/DotLangVisitor.h>
#include <Synthesis/nodes/Commands.h>
#include <Synthesis/nodes/NodeNames.h>

using namespace Synthesis;


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

// ___

void CDotLangVisitor::Visit( const CConditionalJumpCommand* command ) {
    std::string nodeName = generateNodeName( CNodeNames::CJUMP );
    visitedNodeStack.push_back( nodeName );

    command->LeftPart()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    command->RightPart()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CJumpCommand* command ) {
    std::string nodeName = generateNodeName( CNodeNames::JUMP );
    visitedNodeStack.push_back( nodeName );

    std::string labelNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    labelNodeName += command->LabelName() + "\"";
    addEdge( nodeName, labelNodeName );
}

void CDotLangVisitor::Visit( const CCallFunctionCommand* command ) {
    std::string nodeName = generateNodeName( CNodeNames::CALL );
    visitedNodeStack.push_back( nodeName );

    std::string functionNodeName = "\"" + generateNodeName( "Function" ) + ": ";
    functionNodeName += command->Function() + "\"";
    addEdge( nodeName, functionNodeName );

    for ( auto argument : command->Arguments() ) {
        argument->Accept( this );
        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }
}

void CDotLangVisitor::Visit( const CLabelDeclarationCommand* command ) {
    std::string nodeName = generateNodeName( CNodeNames::LABEL );
    visitedNodeStack.push_back( nodeName );

    std::string labelNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    labelNodeName += command->Name() + "\"";
    addEdge( nodeName, labelNodeName );
}

void CDotLangVisitor::Visit( const CMoveRegisterCommand* command ) {
    std::string nodeName = generateNodeName( CNodeNames::MOVE_REGISTER );
    visitedNodeStack.push_back( nodeName );

    command->Destination()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    command->Source()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::TEMP );
    visitedNodeStack.push_back( nodeName );

    std::string tempNodeName = "\"" + generateNodeName( "Temp" ) + ": ";
    tempNodeName += expression->Name() + "\"";
    addEdge( nodeName, tempNodeName );
}

void CDotLangVisitor::Visit( const CAddCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::ADD );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CAddConstCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::ADD_CONST );
    visitedNodeStack.push_back( nodeName );

    expression->Expression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Constant() ) + "\"";
    addEdge( nodeName, valueNodeName );
}

void CDotLangVisitor::Visit( const CSubCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::SUB );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CSubConstCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::SUB_CONST );
    visitedNodeStack.push_back( nodeName );

    expression->Expression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Constant() ) + "\"";
    addEdge( nodeName, valueNodeName );
}

void CDotLangVisitor::Visit( const CMultCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::MULT );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CDivCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::DIV );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CLoadCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::LOAD );
    visitedNodeStack.push_back( nodeName );

    expression->Source()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Offset() ) + "\"";
    addEdge( nodeName, valueNodeName );
}

void CDotLangVisitor::Visit( const CNullExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::NULL_EXP );
    visitedNodeStack.push_back( nodeName );
}

void CDotLangVisitor::Visit( const CExpStatementCommand* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_STAT );
    visitedNodeStack.push_back( nodeName );

    statement->Expression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();
}

void CDotLangVisitor::Visit( const CStoreCommand* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::STORE );
    visitedNodeStack.push_back( nodeName );

    expression->Destination()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Offset() ) + "\"";
    addEdge( nodeName, valueNodeName );

    expression->Source()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName, valueNodeName );
}
