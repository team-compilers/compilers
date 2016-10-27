#pragma once

#include <MethodDeclaration.h>
#include <VisitorTarget.h>
#include <Visitor.h>

class CMethodDeclarationList : public IVisitorTarget {
public:

    CMethodDeclarationList() {}

    Add( const CMethodDeclaration* declaration ) 
        { declarations.push_back( declaration ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CMethodDeclaration> > declarations;
}
