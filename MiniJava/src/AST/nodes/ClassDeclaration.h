#pragma once

#include <memory>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/Expression.h>
#include <AST/nodes/MethodDeclarationList.h>
#include <AST/nodes/VarDeclarationList.h>

namespace ASTree {

class CClassDeclaration : public CVisitorTarget {
public:
    CClassDeclaration( const CIdExpression* _className, const CVarDeclarationList* _varDelcs,
            const CMethodDeclarationList* _methodDecls, const CLocation& _location )
        : CVisitorTarget( _location ),
          className( _className ),
          varDecls( _varDelcs ),
          methodDecls( _methodDecls ),
          hasParent( false ) {}

    CClassDeclaration( const CIdExpression* _className, const CVarDeclarationList* _varDecls,
            const CMethodDeclarationList* _methodDecls, const CIdExpression* parent, const CLocation& _location )
        : CVisitorTarget( _location ),
          className( _className ),
          varDecls( _varDecls ),
          methodDecls( _methodDecls ),
          hasParent( true ),
          extendsClassName( parent ) {}

    const CIdExpression* ClassName() const { return className.get(); }
    const CVarDeclarationList* VarDeclarations() const { return varDecls.get(); }
    const CMethodDeclarationList* MethodDeclarations() const { return methodDecls.get(); }
    bool HasParent() const { return hasParent; }
    const CIdExpression* ExtendsClassName() const { return extendsClassName.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> className;
    std::unique_ptr<const CVarDeclarationList> varDecls;
    std::unique_ptr<const CMethodDeclarationList> methodDecls;
    bool hasParent;
    std::unique_ptr<const CIdExpression> extendsClassName;
};

}
