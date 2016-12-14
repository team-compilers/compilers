#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/astNodes/VisitorTarget.h>

#include <AST/astNodes/Expression.h>
#include <AST/astNodes/TypeModifier.h>
#include <AST/astNodes/VarDeclaration.h>

namespace AstTree {

class CVarDeclarationList : public CVisitorTarget {
public:

    CVarDeclarationList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    void Add( const CVarDeclaration* varDeclaration ) 
        { varDeclarations.emplace_back( varDeclaration ); }

    const std::vector< std::unique_ptr<const CVarDeclaration> >& VarDeclarations() const { return varDeclarations; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CVarDeclaration> > varDeclarations;
};

}
