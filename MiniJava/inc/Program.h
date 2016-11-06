#pragma once

#include <memory>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <ClassDeclarationList.h>
#include <MainClass.h>

class CProgram : public IVisitorTarget {
public:

    CProgram( const CMainClass* _mainClass, const CClassDeclarationList* _classDeclarations )
        : mainClass( _mainClass ),
          classDeclarations( _classDeclarations ) {}

    const CMainClass* MainClass() const { return mainClass.get(); }
    const CClassDeclarationList* ClassDeclarations() const { return classDeclarations.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CMainClass> mainClass;
    std::unique_ptr<const CClassDeclarationList> classDeclarations;
};
