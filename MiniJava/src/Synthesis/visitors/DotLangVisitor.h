#pragma once

#include <Synthesis/visitors/Visitor.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Synthesis {

class CDotLangVisitor : public CVisitor {
public:
    using CVisitor::CVisitor;
    virtual ~CDotLangVisitor() {}

    // Generates a string, representing the last traversal of the tree, in the DOT Language.
    // The DOT Language is the one supported by GraphViz.
    std::string GetTraversalInDotLanguage() const;

    void Visit( const CConditionalJumpCommand* command ) override;
    void Visit( const CJumpCommand* command ) override;
    void Visit( const CCallFunctionCommand* command ) override;
    void Visit( const CLabelDeclarationCommand* command ) override;
    void Visit( const CMoveRegisterCommand* command ) override;
    void Visit( const CTempExpression* expression ) override;
    void Visit( const CAddCommand* expression ) override;
    void Visit( const CAddConstCommand* expression ) override;
    void Visit( const CSubCommand* expression ) override;
    void Visit( const CSubConstCommand* expression ) override;
    void Visit( const CMultCommand* expression ) override;
    void Visit( const CDivCommand* expression ) override;
    void Visit( const CLoadCommand* expression ) override;
    void Visit( const CStoreCommand* expression ) override;
    void Visit( const CNullExpression* expression ) override;
    void Visit( const CExpStatementCommand* statement ) override;

private:
    std::unordered_map<std::string, std::vector<std::string>> treeEdges;
    // used to track the traverse and collect some information
    std::vector<std::string> visitedNodeStack;

    // Adds edge (nodeFromName; nodeToName) to treeEdges.
    void addEdge( const std::string& nodeFromName, const std::string& nodeToName );
};

}; // Synthesis
