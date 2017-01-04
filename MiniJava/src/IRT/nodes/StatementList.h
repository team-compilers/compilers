#pragma once

#include <memory>
#include <vector>

#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class CStatement;

class CStatementList : public IVisitorTarget {
public:
    CStatementList() = default;

    CStatementList( const CStatement* statement ) { Add( statement ); }

    void Add( const CStatement* statement )
        { statements.emplace_back( statement ); }

    const std::vector< std::unique_ptr<const CStatement> >& Statements() const { return statements; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CStatement> > statements;
};

}
