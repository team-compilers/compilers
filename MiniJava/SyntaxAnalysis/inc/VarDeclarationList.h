#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <Expression.h>
#include <TypeModifier.h>
#include <VarDeclaration.h>

class CVarDeclarationList : public IVisitorTarget {
public:

    CVarDeclarationList() = default;

    void Add( const CVarDeclaration* varDeclaration ) 
        { varDeclarations.emplace_back( varDeclaration ); }

    const std::vector< std::unique_ptr<const CVarDeclaration> >& VarDeclarations() const { return varDeclarations; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CVarDeclaration> > varDeclarations;
};
