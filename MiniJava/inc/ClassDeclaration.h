#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <VarDeclarationList.h>
#include <MethodDeclarationList.h>
#include <Expression.h>
#include <memory>

class CClassDeclaration : public IVisitorTarget {
public:

    CClassDeclaration( const CIdExpression* _className, const CVarDeclarationList* _varDelcs, 
            const CMethodDeclarationList* _methodDecls )
        : className( _className ),
          varDecls( _varDelcs ),
          methodDecls( _methodDecls ),
          hasParent( false ) {}

    CClassDeclaration( const CIdExpression* _className, const CVarDeclarationList* _varDecls,
            const CMethodDeclarationList* _methodDecls, const CIdExpression* parent ) 
        : className( _className ),
          varDecls( _varDecls ),
          methodDecls( _methodDecls ),
          hasParent( true ),
          extendsClassName( parent ) {}

    const CIdExpression* ClassName() const { return className.get(); }
    const CVarDeclarationList* VarDeclarations() const { return varDecls.get(); }
    const CMethodDeclarationList* MethodDeclarations() const { return methodDecls.get(); }
    bool HasParent() const { return hasParent; }
    const CIdExpression* ExtendsClassName() const { return extendsClassName.get(); } 

private:
    std::unique_ptr<const CIdExpression> className;
    std::unique_ptr<const CVarDeclarationList> varDecls;
    std::unique_ptr<const CMethodDeclarationList> methodDecls;
    bool hasParent;
    std::unique_ptr<const CIdExpression> extendsClassName;
};
