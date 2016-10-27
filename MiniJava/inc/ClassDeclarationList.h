#pragma once

#include <ClassDeclaration.h>
#include <Visitor.h>
#include <VisitorTarget.h>
#include <vector>
#include <memory>

class CClassDeclarationList : public IVisitorTarget {
public:

    CClassDeclarationList() = default;

    Add( const CClassDeclaration* declaration ) 
        { classes.push_back( declaration ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CClassDeclaration> > classes;
}
