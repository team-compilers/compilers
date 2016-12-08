#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <MethodDeclaration.h>

namespace AstTree {

class CMethodDeclarationList : public CVisitorTarget {
public:

    CMethodDeclarationList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    void Add( const CMethodDeclaration* declaration ) 
        { declarations.emplace_back( declaration ); }
    
    const std::vector< std::unique_ptr<const CMethodDeclaration> >& MethodDeclarations() const { return declarations; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CMethodDeclaration> > declarations;
};

}
