#pragma once

#include <memory>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <Expression.h>
#include <StatementList.h>

class CMainClass : public CVisitorTarget {
public:
    CMainClass( const CIdExpression* _className, const CIdExpression* _classArgsName, const CStatementList* _statements, const CLocation& _location )
        : CVisitorTarget( _location ),
          className( _className ),
          classArgsName( _classArgsName ),
          statements( _statements ) {}

    const CIdExpression* ClassName() const { return className.get(); }
    const CIdExpression* ClassArgsName() const { return classArgsName.get(); }
    const CStatementList* Statements() const { return statements.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const CIdExpression> className;
    std::unique_ptr<const CIdExpression> classArgsName;
    std::unique_ptr<const CStatementList> statements;
};