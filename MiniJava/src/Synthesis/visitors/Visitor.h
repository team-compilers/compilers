#pragma once

namespace Synthesis {

class CAddCommand;

class IVisitor {
public:
    virtual ~IVisitor() = 0;

    virtual void Visit( const CAddCommand* expression ) = 0;
};;

class CVisitor : public IVisitor {
public:
    CVisitor( bool _verbose ) : verbose( _verbose ) {}
    virtual ~CVisitor() {}

protected:
    bool verbose;
};

}
