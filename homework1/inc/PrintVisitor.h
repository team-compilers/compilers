// Author: Andrew Sautin
// Description: PrintVisitor

#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Visitor.h>

class CPrintVisitor : public IVisitor {
    std::unordered_map<std::string, std::vector<std::string>> treeEdges;
    std::unordered_map<std::string, int> nodeTypeLastUsedIndex;
    std::stack<std::string> visitedNodeStack;

    // generates unique id for nodes of one type
    int generateNodeNextIndex( const std::string& nodeType );

    // generates full node name based on the node type and a unique id assigned to the node
    std::string generateNodeName( const std::string& nodeTypeName );

    // adds edge (nodeFromName; nodeToName) to treeEdges
    void addEdge( const std::string& nodeFromName, const std::string& nodeToName );

    std::string toString( const TOperandType& type ) const;

    void Visit( CBinaryExpression* expression ) override;
    void Visit( CNumberExpression* expression ) override;
    void Visit( CIdExpression* expression ) override;
    void Visit( CPairListExpression* expression ) override;
    void Visit( CSingleElementListExpression* expression ) override;
    void Visit( CPrintStatement* statement ) override;
    void Visit( CAssignStatement* statement ) override;
    void Visit( CCompoundStatement* statement ) override;
};
