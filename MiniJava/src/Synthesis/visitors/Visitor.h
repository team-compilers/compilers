#pragma once

#include <string>
#include <unordered_map>

namespace Synthesis {

class CAddCommand;
class CAddConstCommand;
class CSubCommand;
class CSubConstCommand;
class CMultCommand;
class CDivCommand;
class CLoadCommand;
class CStoreCommand;
class CNullExpression;
class CTempExpression;
class CMoveRegisterCommand;
class CLabelDeclarationCommand;
class CCallFunctionCommand;
class CJumpCommand;
class CConditionalJumpCommand;
class CExpStatementCommand;

class IVisitor {
public:
    virtual ~IVisitor() {}

    virtual void Visit( const CConditionalJumpCommand* command ) = 0;
    virtual void Visit( const CJumpCommand* command ) = 0;
    virtual void Visit( const CCallFunctionCommand* command ) = 0;
    virtual void Visit( const CLabelDeclarationCommand* command ) = 0;
    virtual void Visit( const CMoveRegisterCommand* command ) = 0;
    virtual void Visit( const CTempExpression* expression ) = 0;
    virtual void Visit( const CAddCommand* expression ) = 0;
    virtual void Visit( const CAddConstCommand* expression ) = 0;
    virtual void Visit( const CSubCommand* expression ) = 0;
    virtual void Visit( const CSubConstCommand* expression ) = 0;
    virtual void Visit( const CMultCommand* expression ) = 0;
    virtual void Visit( const CDivCommand* expression ) = 0;
    virtual void Visit( const CLoadCommand* expression ) = 0;
    virtual void Visit( const CStoreCommand* expression ) = 0;
    virtual void Visit( const CNullExpression* expression ) = 0;
    virtual void Visit( const CExpStatementCommand* statement ) = 0;
};

class CVisitor : public IVisitor {
public:
    CVisitor( bool _verbose ) : verbose( _verbose ) {}
    virtual ~CVisitor() {}

    // Generates unique id for nodes of one type.
    int generateNodeNextIndex( const std::string& nodeType );

    // Generates full node name based on the node type and a unique id assigned to the node.
    std::string generateNodeName( const std::string& nodeTypeName );

protected:
    bool verbose;
    // used for generating unique node names
    std::unordered_map<std::string, int> nodeTypeLastUsedIndex;
};

}
