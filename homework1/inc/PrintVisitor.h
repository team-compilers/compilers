// Author: Andrew Sautin
// Description: PrintVisitor

#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Visitor.h>

class CPrintVisitor : public IVisitor {
public:
    // Generates a string, representing the last traversal of the tree, in the DOT Language.
    // The DOT Language is the one supported by GraphViz.
    std::string GetTraversalInDotLanguage() const;

    // Resets the initial state of the object.
    // Should be used before every tree traversal (except the first one).
    void Clear();

    // Visitors for different node types.
    void Visit( CBinaryExpression* expression ) override;
    void Visit( CNumberExpression* expression ) override;
    void Visit( CIdExpression* expression ) override;
    void Visit( CPairListExpression* expression ) override;
    void Visit( CSingleElementListExpression* expression ) override;
    void Visit( CPrintStatement* statement ) override;
    void Visit( CAssignStatement* statement ) override;
    void Visit( CCompoundStatement* statement ) override;
private:
    std::unordered_map<std::string, std::vector<std::string>> treeEdges;
    std::unordered_map<std::string, int> nodeTypeLastUsedIndex;
    std::vector<std::string> visitedNodeStack;

    // Generates unique id for nodes of one type.
    int generateNodeNextIndex( const std::string& nodeType );

    // Generates full node name based on the node type and a unique id assigned to the node.
    std::string generateNodeName( const std::string& nodeTypeName );

    // Adds edge (nodeFromName; nodeToName) to treeEdges.
    void addEdge( const std::string& nodeFromName, const std::string& nodeToName );

    // Maps each TOperandType to a string.
    std::string toString( const TOperandType& type ) const;
};
