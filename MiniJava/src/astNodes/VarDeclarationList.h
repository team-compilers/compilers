#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <astNodes/VisitorTarget.h>

#include <astNodes/Expression.h>
#include <astNodes/TypeModifier.h>
#include <astNodes/VarDeclaration.h>

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
