#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <Statement.h>
#include <memory>
#include <vector>

class CStatementList : public IVisitorTarget {
public:

    CStatementList() = default;

    void Add( const IStatement* statement ) 
        { statements.emplace_back( statement ); }
    
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const IStatement> > statements;
};
