#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <TypeModifier.h>
#include <Expression.h>
#include <VarDeclaration.h>
#include <vector>
#include <memory>

class CVarDeclarationList : public IVisitorTarget {
public:

    CVarDeclarationList() = default;

    void Add( const CVarDeclaration* varDeclaration ) 
        { varDeclarations.emplace_back( varDeclaration ); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CVarDeclaration> > varDeclarations;
};
