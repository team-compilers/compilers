#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <Expression.h>
#include <Statement.h>
#include <memory>

class CMainClass : public IVisitorTarget {
public:
    CMainClass( const CIdExpression* _className, const CIdExpression* _classArgsName, const CStatementList* _statement )
        : className( _className ),
          classArgsName( _classArgsName ),
          statement( _statement ) {}

    const CIdExpression* ClassName() const { return className.get(); }
    const CIdExpression* ClassArgsName() const { return classArgsName.get(); }
    const CStatementList* Statement() const { return statement.get(); }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> className;
    std::unique_ptr<const CIdExpression> classArgsName;
    std::unique_ptr<const CStatementList> statement;
}
