#pragma once

#include <memory>

#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class CLabelStatement;

class CLabelList : public IVisitorTarget {
public:
    CLabelList() = default;

    CLabelList( const CLabelStatement* statement ) { Add( statement ); }

    void Add( const CLabelStatement* label )
        { labels.emplace_back( label ); }

    const std::vector< std::unique_ptr<const CLabelStatement> >& Labels() const { return labels; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CLabelStatement> > labels;
};

} // namespace IRTree
