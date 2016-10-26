#pragma once

#include <Statement.h>

class CStatementList : public IVisitorTarget {
public:

    CStatementList() {}

    Add( const IStatement* statement ) 
        { statements.push_back( statement ); }
    
    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const IStatement> > statements;
}
