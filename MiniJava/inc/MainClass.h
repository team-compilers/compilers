#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <Expression.h>
#include <Statement.h>

class CMainClass : public IVisitorTarget {
public:
    CMainClass( const CIdExpression* _className, const CIdExpression* _classArgsName, const IStatement* _statement )
        : className( _className ),
          classArgsName( _classArgsName ),
          statement( _statement ) {}

    const CIdExpression* ClassName() const { return className; }
    const CIdExpression* ClassArgsName() const { return classArgsName; }
    const IStatement* Statement() const { return statement; }

    void Accept( IVisitor* visitor) override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> className;
    std::unique_ptr<const CIdExpression> classArgsName;
    std::unique_ptr<const IStatement> statement;
}
