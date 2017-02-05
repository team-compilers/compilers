// Author: Alexey Zhuravlev && Andrew Sautin
// Description: Visitor interface

#pragma once

#include <iostream> // for verbose output
#include <string>
#include <unordered_map>
#include <vector>

namespace ASTree {

class CPublicAccessModifier;
class CPrivateAccessModifier;

class CBinaryExpression;
class CBracketExpression;
class CNumberExpression;
class CLogicExpression;
class CIdExpression;
class CLengthExpression;
class CMethodExpression;
class CThisExpression;
class CNewArrayExpression;
class CNewIdExpression;
class CNegateExpression;

class CAssignIdStatement;
class CAssignIdWithIndexStatement;
class CPrintStatement;
class CConditionalStatement;
class CWhileLoopStatement;
class CBracesStatement;

class CIntTypeModifier;
class CBooleanTypeModifier;
class CIntArrayTypeModifier;
class CIdTypeModifier;

class CVarDeclaration;
class CMethodArgument;
class CMethodDeclaration;
class CMainClass;
class CClassDeclaration;
class CProgram;

class CExpressionList;
class CStatementList;
class CVarDeclarationList;
class CMethodArgumentList;
class CMethodDeclarationList;
class CClassDeclarationList;

class CLocation;

class IVisitor {
public:
    virtual ~IVisitor() {}
    virtual void Visit( const CPublicAccessModifier* modifier ) = 0;
    virtual void Visit( const CPrivateAccessModifier* modifier ) = 0;

    virtual void Visit( const CBinaryExpression* expression ) = 0;
    virtual void Visit( const CBracketExpression* expression ) = 0;
    virtual void Visit( const CNumberExpression* expression ) = 0;
    virtual void Visit( const CLogicExpression* expression ) = 0;
    virtual void Visit( const CIdExpression* expression ) = 0;
    virtual void Visit( const CLengthExpression* expression ) = 0;
    virtual void Visit( const CMethodExpression* expression ) = 0;
    virtual void Visit( const CThisExpression* expression ) = 0;
    virtual void Visit( const CNewArrayExpression* expression ) = 0;
    virtual void Visit( const CNewIdExpression* expression ) = 0;
    virtual void Visit( const CNegateExpression* expression ) = 0;

    virtual void Visit( const CAssignIdStatement* statement ) = 0;
    virtual void Visit( const CAssignIdWithIndexStatement* statement ) = 0;
    virtual void Visit( const CPrintStatement* statement ) = 0;
    virtual void Visit( const CConditionalStatement* statement ) = 0;
    virtual void Visit( const CWhileLoopStatement* statement ) = 0;
    virtual void Visit( const CBracesStatement* statement ) = 0;

    virtual void Visit( const CIntTypeModifier* typeModifier ) = 0;
    virtual void Visit( const CBooleanTypeModifier* typeModifier ) = 0;
    virtual void Visit( const CIntArrayTypeModifier* typeModifier ) = 0;
    virtual void Visit( const CIdTypeModifier* typeModifier ) = 0;

    virtual void Visit( const CVarDeclaration* declaration ) = 0;
    virtual void Visit( const CMethodArgument* argument ) = 0;
    virtual void Visit( const CMethodDeclaration* declaration ) = 0;
    virtual void Visit( const CMainClass* mainClass ) = 0;
    virtual void Visit( const CClassDeclaration* declaration ) = 0;
    virtual void Visit( const CProgram* program ) = 0;

    virtual void Visit( const CExpressionList* list ) = 0;
    virtual void Visit( const CStatementList* list ) = 0;
    virtual void Visit( const CVarDeclarationList* list ) = 0;
    virtual void Visit( const CMethodArgumentList* list ) = 0;
    virtual void Visit( const CMethodDeclarationList* list ) = 0;
    virtual void Visit( const CClassDeclarationList* list ) = 0;
};

enum class TOperatorType : char;

class CVisitor : public IVisitor {
public:
    CVisitor( bool _verbose ) : verbose( _verbose ) {}
protected:
    // Generates unique id for nodes of one type.
    int generateNodeNextIndex( const std::string& nodeType );

    // Generates full node name based on the node type and a unique id assigned to the node.
    std::string generateNodeName( const std::string& nodeTypeName );

    // Is called every time a visitor enters a node
    virtual void onNodeEnter( const std::string& nodeName, const CLocation& location );
    // Is called every time a visitor is about to exit a node
    virtual void onNodeExit( const std::string& nodeName, const CLocation& location );

    // Maps each TOperatorType to a string with its name.
    std::string operatorName( const TOperatorType& type ) const;

    // Maps each TOperatorType to a string with its character.
    std::string operatorChar( const TOperatorType& type ) const;

    // used for generating unique node names
    std::unordered_map<std::string, int> nodeTypeLastUsedIndex;

    // used for verbose output while traversing the tree
    bool verbose;
    const std::string nodeEnterMarker = "in: ";
    const std::string nodeExitMarker = "out: ";
    std::string margin;
};

}
