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
#include <Expression.h>
// #include <Statement.h>

class CPrintVisitor : public IVisitor {
public:
    CPrintVisitor() = default;
    ~CPrintVisitor() {}
    // Generates a string, representing the last traversal of the tree, in the DOT Language.
    // The DOT Language is the one supported by GraphViz.
    std::string GetTraversalInDotLanguage() const;

    // Resets the initial state of the object.
    // Should be used before every tree traversal (except the first one).
    void Clear();

    // Visitors for different node types.
    void Visit( CPublicAccessModifier* modifier ) override;
    void Visit( CPrivateAccessModifier* modifier ) override;

    void Visit( CBinaryExpression* expression ) override;
    void Visit( CBracketExpression* expression ) override;
    void Visit( CNumberExpression* expression ) override;
    void Visit( CLogicExpression* expression ) override;
    void Visit( CIdExpression* expression ) override;
    void Visit( CLengthExpression* expression ) override;
    void Visit( CMethodExpression* expression ) override;
    void Visit( CThisExpression* expression ) override;
    void Visit( CNewArrayExpression* expression ) override;
    void Visit( CNewIdExpression* expression ) override;
    void Visit( CNegateExpression* expression ) override;

    void Visit( CAssignIdStatement* statement ) override;
    void Visit( CAssignIdWithIndexStatement* statement ) override;
    void Visit( CPrintStatement* statement ) override;
    void Visit( CConditionalStatement* statement ) override;
    void Visit( CWhileLoopStatement* statement ) override;
    void Visit( CBracesStatement* statement ) override;

    void Visit( CIntTypeModifier* typeModifier ) override;
    void Visit( CBooleanTypeModifier* typeModifier ) override;
    void Visit( CIntArrayTypeModifier* typeModifier ) override;
    void Visit( CIdTypeModifier* typeModifier ) override;

    void Visit( CVarDeclaration* declaration ) override;
    void Visit( CMethodArgument* argument ) override;
    void Visit( CMethodDeclaration* declaration ) override;
    void Visit( CMainClass* mainClass ) override;
    void Visit( CClassDeclaration* declaration ) override;
    void Visit( CProgram* program ) override;

    void Visit( CExpressionList* list ) override;
    void Visit( CStatementList* list ) override;
    void Visit( CVarDeclarationList* list ) override;
    void Visit( CMethodArgumentList* list ) override;
    void Visit( CMethodDeclarationList* list ) override;
    void Visit( CClassDeclarationList* list ) override;

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
