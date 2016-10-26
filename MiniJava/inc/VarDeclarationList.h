#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <TypeModifier.h>
#include <Expression.h>
#include <vector>

class CVarDeclaration : public IVisitorTarget {
public:

    CVarDeclaration( const ITypeModifier* _type, const CIdExpression* _id )
        : type( _type ),
          id( _id ) {}

    const ITypeModifier* Type() const { return type; }
    const CIdExpression* Id() const { return id; }

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::unique_ptr<const ITypeModifier> type;
    std::unique_ptr<const CIdExpression> id;
};

//---------------------------------------------------------------------------------------//

class CVarDeclarationList : public IVisitorTarget {
public:

    CVarDeclarationList() {}

    void Add( const CVarDeclarationList* varDeclaration ) 
        { varDeclarations.push_back( varDelaration ); }

private:
    std::vector< std::unique_ptr<const CVarDeclaration> > varDeclarations;
}
