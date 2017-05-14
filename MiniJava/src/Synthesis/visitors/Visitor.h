#pragma once

namespace Synthesis {

class CAddCommand;
class CAddConstCommand;
class CSubCommand;
class CSubConstCommand;
class CMultCommand;
class CDivCommand;
class CLoadCommand;
class CStoreCommand;
class CMoveCommand;

class IVisitor {
public:
    virtual ~IVisitor() = 0;

    virtual void Visit( const CAddCommand* expression ) = 0;
    virtual void Visit( const CAddConstCommand* expression ) = 0;
    virtual void Visit( const CSubCommand* expression ) = 0;
    virtual void Visit( const CSubConstCommand* expression ) = 0;
    virtual void Visit( const CMultCommand* expression ) = 0;
    virtual void Visit( const CDivCommand* expression ) = 0;
    virtual void Visit( const CLoadCommand* expression ) = 0;
    virtual void Visit( const CStoreCommand* expression ) = 0;
    virtual void Visit( const CMoveCommand* expression ) = 0;
};;

class CVisitor : public IVisitor {
public:
    CVisitor( bool _verbose ) : verbose( _verbose ) {}
    virtual ~CVisitor() {}

protected:
    bool verbose;
};

}
