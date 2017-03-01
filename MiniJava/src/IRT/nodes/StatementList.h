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
    CStatementList( std::unique_ptr<const CStatement> statement ) { Add( std::move( statement ) ); }

    void Add( const CStatement* statement )
        { statements.emplace_back( statement ); }
    void Add( std::unique_ptr<const CStatement> statement )
        { statements.push_back( std::move( statement ) ); }

    const std::vector< std::unique_ptr<const CStatement> >& Statements() const { return statements; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    std::unique_ptr<const CStatementList> Clone() const;
    std::unique_ptr<const CStatementList> Canonize() const;

private:
    std::vector<std::unique_ptr<const CStatement>> statements;
};

}
