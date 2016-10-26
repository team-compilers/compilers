#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <TypeModifier.h>
#include <Expression.h>
#include <VarDeclaration.h>
#include <vector>

class CVarDeclarationList : public IVisitorTarget {
public:

    CVarDeclarationList() {}

    void Add( const CVarDeclarationList* varDeclaration ) 
        { varDeclarations.push_back( varDelaration ); }

private:
    std::vector< std::unique_ptr<const CVarDeclaration> > varDeclarations;
}
