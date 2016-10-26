#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <MainClass.h>
#include <ClassDeclarationList.h>
#include <memory>

class CProgram : public IVisitorTarget {
public:

    CProgram( const CMainClass* _mainClass, const CClassDeclarationList* _classDeclarations )
        : mainClass( _mainClass ),
          classDeclarations( _classDeclarations ) {}

    const CMainClass* MainClass() const { return mainClass; }
    const CClassDeclarationList* ClassDeclarations() const { return classDeclarations; }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }

private:

    std::unique_ptr<const CMainClass> mainClass;
    std::unique_ptr<const CClassDeclarationList> classDeclarations;
};
