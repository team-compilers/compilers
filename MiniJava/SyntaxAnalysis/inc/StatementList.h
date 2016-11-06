#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

class IStatement;

class CStatementList : public IVisitorTarget {
public:

    CStatementList() = default;

    void Add( const IStatement* statement ) 
        { statements.emplace_back( statement ); }

    const std::vector< std::unique_ptr<const IStatement> >& Statements() const { return statements; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
	// must be reversed before being used
    std::vector< std::unique_ptr<const IStatement> > statements;
};
