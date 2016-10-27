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

    CVarDeclarationList() {}

    void Add( const CVarDeclarationList* varDeclaration ) 
        { varDeclarations.push_back( varDelaration ); }

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CVarDeclaration> > varDeclarations;
}
