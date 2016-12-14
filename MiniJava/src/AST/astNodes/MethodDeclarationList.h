#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/astNodes/VisitorTarget.h>

#include <AST/astNodes/MethodDeclaration.h>

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
