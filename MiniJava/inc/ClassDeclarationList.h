#pragma once

#include <ClassDeclaration.h>
#include <VisitorTarget.h>

class CClassDeclarationList : public IVisitorTarget {
public:

    CClassDeclarationList() {}

    Add( const CClassDeclaration* declaration ) 
        { classes.push_back( declaration ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CClassDeclaration> > classes;
}
