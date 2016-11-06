#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <ClassDeclaration.h>

class CClassDeclarationList : public IVisitorTarget {
public:

    CClassDeclarationList() = default;

    void Add( const CClassDeclaration* declaration ) 
        { classes.emplace_back( declaration ); }
    
    const std::vector< std::unique_ptr<const CClassDeclaration> >& ClassDeclarations() const { return classes; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CClassDeclaration> > classes;
};
