#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/nodes/VisitorTarget.h>

#include <AST/nodes/Expression.h>
#include <AST/nodes/MethodArgument.h>
#include <AST/nodes/TypeModifier.h>

namespace ASTree {

class CMethodArgumentList : public CVisitorTarget {
public:

    CMethodArgumentList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    void Add( const CMethodArgument* argument )
        { arguments.emplace_back( argument ); }

    const std::vector<std::unique_ptr<const CMethodArgument>>& MethodArguments() const { return arguments; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CMethodArgument> > arguments;
};

}
