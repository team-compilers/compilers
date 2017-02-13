#pragma once

#include <iostream> // for verbose output
#include <string>
#include <unordered_map>
#include <vector>

namespace IRTree {

class CConstExpression;
class CNameExpression;
class CTempExpression;
class CBinaryExpression;
class CMemExpression;
class CCallExpression;
class CEseqExpression;

class CExpStatement;
class CJumpConditionalStatement;
class CJumpStatement;
class CLabelStatement;
class CMoveStatement;
class CSeqStatement;

class CExpressionList;
class CStatementList;

class IVisitor {
public:
    virtual ~IVisitor() {}

    virtual void Visit( const CConstExpression* expression ) = 0;
    virtual void Visit( const CNameExpression* expression ) = 0;
    virtual void Visit( const CTempExpression* expression ) = 0;
    virtual void Visit( const CBinaryExpression* expression ) = 0;
    virtual void Visit( const CMemExpression* expression ) = 0;
    virtual void Visit( const CCallExpression* expression ) = 0;
    virtual void Visit( const CEseqExpression* expression ) = 0;

    virtual void Visit( const CExpStatement* expression ) = 0;
    virtual void Visit( const CJumpConditionalStatement* expression ) = 0;
    virtual void Visit( const CJumpStatement* expression ) = 0;
    virtual void Visit( const CLabelStatement* expression ) = 0;
    virtual void Visit( const CMoveStatement* expression ) = 0;
    virtual void Visit( const CSeqStatement* expression ) = 0;

    virtual void Visit( const CExpressionList* list ) = 0;
    virtual void Visit( const CStatementList* list ) = 0;
};

enum class TOperatorType : char;
enum class TLogicOperatorType : char;

class CVisitor : public IVisitor {
public:
    CVisitor( bool _verbose ) : verbose( _verbose ) {}
    virtual ~CVisitor() {}
protected:
    // Generates unique id for nodes of one type.
    int generateNodeNextIndex( const std::string& nodeType );

    // Generates full node name based on the node type and a unique id assigned to the node.
    std::string generateNodeName( const std::string& nodeTypeName );

    // Is called every time visitor enters a node
    void onNodeEnter( const std::string& nodeName );
    // Is called every time visitor is about to exit a node
    void onNodeExit( const std::string& nodeName );

    // Maps each TOperatorType to a string with its name.
    std::string operatorName( const TOperatorType& type ) const;
    // Maps each TLogicOperatorType to a string with its name.
    std::string operatorName( const TLogicOperatorType& type ) const;

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
