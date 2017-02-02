#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

namespace ASTree {

class CStatement;

class CStatementList : public CVisitorTarget {
public:

    CStatementList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    void Add( const CStatement* statement )
        { statements.emplace_back( statement ); }

    const std::vector< std::unique_ptr<const CStatement> >& Statements() const { return statements; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    // must be reversed before being used
    std::vector< std::unique_ptr<const CStatement> > statements;
};

}
