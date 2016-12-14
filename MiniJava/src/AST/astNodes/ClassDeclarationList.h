#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/astNodes/VisitorTarget.h>

#include <AST/astNodes/ClassDeclaration.h>

namespace AstTree {

class CClassDeclarationList : public CVisitorTarget {
public:
    CClassDeclarationList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    void Add( const CClassDeclaration* declaration ) 
        { classes.emplace_back( declaration ); }
    
    const std::vector< std::unique_ptr<const CClassDeclaration> >& ClassDeclarations() const { return classes; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CClassDeclaration> > classes;
};

}
