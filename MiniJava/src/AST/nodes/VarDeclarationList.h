#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/Expression.h>
#include <AST/nodes/TypeModifier.h>
#include <AST/nodes/VarDeclaration.h>

namespace ASTree {

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
