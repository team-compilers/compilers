// Author: Andrew Sautin
// Description: PrintVisitor

#pragma once

#include <iostream> // for verbose output
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Visitor.h>

#include <AccessModifier.h>
#include <Expression.h>
#include <ExpressionList.h>
#include <Statement.h>
#include <StatementList.h>
#include <TypeModifier.h>
#include <VarDeclaration.h>
#include <VarDeclarationList.h>
#include <MethodArgument.h>
#include <MethodArgumentList.h>
#include <MethodDeclaration.h>
#include <MethodDeclarationList.h>
#include <MainClass.h>
#include <ClassDeclaration.h>
#include <ClassDeclarationList.h>
#include <Program.h>

class CPrintVisitor : public IVisitor {
public:
    CPrintVisitor( bool _verbose = false ) : verbose( _verbose ) {}
    ~CPrintVisitor() {}
    // Generates a string, representing the last traversal of the tree, in the DOT Language.
    // The DOT Language is the one supported by GraphViz.
    std::string GetTraversalInDotLanguage() const;

    // Resets the initial state of the object.
    // Should be used before every tree traversal (except the first one).
    void Clear();

    // Visitors for different node types.
    void Visit( const CPublicAccessModifier* modifier ) override;
    void Visit( const CPrivateAccessModifier* modifier ) override;

    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CBracketExpression* expression ) override;
    void Visit( const CNumberExpression* expression ) override;
    void Visit( const CLogicExpression* expression ) override;
    void Visit( const CIdExpression* expression ) override;
    void Visit( const CLengthExpression* expression ) override;
    void Visit( const CMethodExpression* expression ) override;
    void Visit( const CThisExpression* expression ) override;
    void Visit( const CNewArrayExpression* expression ) override;
    void Visit( const CNewIdExpression* expression ) override;
    void Visit( const CNegateExpression* expression ) override;

    void Visit( const CAssignIdStatement* statement ) override;
    void Visit( const CAssignIdWithIndexStatement* statement ) override;
    void Visit( const CPrintStatement* statement ) override;
    void Visit( const CConditionalStatement* statement ) override;
    void Visit( const CWhileLoopStatement* statement ) override;
    void Visit( const CBracesStatement* statement ) override;

    void Visit( const CIntTypeModifier* typeModifier ) override;
    void Visit( const CBooleanTypeModifier* typeModifier ) override;
    void Visit( const CIntArrayTypeModifier* typeModifier ) override;
    void Visit( const CIdTypeModifier* typeModifier ) override;

    void Visit( const CVarDeclaration* declaration ) override;
    void Visit( const CMethodArgument* argument ) override;
    void Visit( const CMethodDeclaration* declaration ) override;
    void Visit( const CMainClass* mainClass ) override;
    void Visit( const CClassDeclaration* declaration ) override;
    void Visit( const CProgram* program ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;
    void Visit( const CVarDeclarationList* list ) override;
    void Visit( const CMethodArgumentList* list ) override;
    void Visit( const CMethodDeclarationList* list ) override;
    void Visit( const CClassDeclarationList* list ) override;

private:
    std::unordered_map<std::string, std::vector<std::string>> treeEdges;
    std::unordered_map<std::string, int> nodeTypeLastUsedIndex;
    std::vector<std::string> visitedNodeStack;

    // used for verbose output while traversing the tree
    bool verbose;
    const std::string nodeEnterMarker = "in: ";
    const std::string nodeExitMarker = "out: ";
    std::string margin;

    // Generates unique id for nodes of one type.
    int generateNodeNextIndex( const std::string& nodeType );

    // Generates full node name based on the node type and a unique id assigned to the node.
    std::string generateNodeName( const std::string& nodeTypeName );

    // Adds edge (nodeFromName; nodeToName) to treeEdges.
    void addEdge( const std::string& nodeFromName, const std::string& nodeToName );

    // Maps each TOperandType to a string.
    std::string toString( const TOperandType& type ) const;

    // Is called every time visitor enters a node
    void onNodeEnter( const std::string& nodeName );
    // Is called every time visitor is about to exit a node
    void onNodeExit( const std::string& nodeName );
};
