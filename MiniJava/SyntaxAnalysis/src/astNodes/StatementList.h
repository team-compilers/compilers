#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

namespace AstTree {

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
