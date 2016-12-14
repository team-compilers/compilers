#pragma once

#include <memory>

#include <AST/visitors/Visitor.h>
#include <AST/astNodes/VisitorTarget.h>

#include <AST/astNodes/ClassDeclarationList.h>
#include <AST/astNodes/MainClass.h>

namespace AstTree {

class CProgram : public CVisitorTarget {
public:

    CProgram( const CMainClass* _mainClass, const CClassDeclarationList* _classDeclarations, const CLocation& _location )
        : CVisitorTarget( _location ),
          mainClass( _mainClass ),
          classDeclarations( _classDeclarations ) {}

    const CMainClass* MainClass() const { return mainClass.get(); }
    const CClassDeclarationList* ClassDeclarations() const { return classDeclarations.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CMainClass> mainClass;
    std::unique_ptr<const CClassDeclarationList> classDeclarations;
};

}
