// Author: Andrew Sautin
// Description: DotLangVisitor

#pragma once

#include <iostream> // for verbose output
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>

namespace IRTree {

class CDotLangVisitor : public CVisitor {
public:
    CDotLangVisitor( bool _verbose = false ) : CVisitor( _verbose ) {}
    ~CDotLangVisitor() {}
    // Generates a string, representing the last traversal of the tree, in the DOT Language.
    // The DOT Language is the one supported by GraphViz.
    std::string GetTraversalInDotLanguage() const;

    // Resets the initial state of the object.
    // Should be used before every tree traversal (except the first one).
    void Clear();

    // Visitors for different node types.
    void Visit( const CConstExpression* expression ) override;
    void Visit( const CNameExpression* expression ) override;
    void Visit( const CTempExpression* expression ) override;
    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CMemExpression* expression ) override;
    void Visit( const CCallExpression* expression ) override;
    void Visit( const CEseqExpression* expression ) override;

    void Visit( const CExpStatement* statement ) override;
    void Visit( const CJumpConditionalStatement* statement ) override;
    void Visit( const CJumpStatement* statement ) override;
    void Visit( const CLabelStatement* statement ) override;
    void Visit( const CMoveStatement* statement ) override;
    void Visit( const CSeqStatement* statement ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;

private:
    std::unordered_map<std::string, std::vector<std::string>> treeEdges;
    // used to track the traverse and collect some information
    std::vector<std::string> visitedNodeStack;

    // Adds edge (nodeFromName; nodeToName) to treeEdges.
    void addEdge( const std::string& nodeFromName, const std::string& nodeToName );
};

}
