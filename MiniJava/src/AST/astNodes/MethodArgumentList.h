#pragma once

#include <memory>
#include <vector>

#include <AST/visitors/Visitor.h>
#include <AST/astNodes/VisitorTarget.h>

#include <AST/astNodes/Expression.h>
#include <AST/astNodes/MethodArgument.h>
#include <AST/astNodes/TypeModifier.h>

namespace AstTree {

class CMethodArgumentList : public CVisitorTarget {
public:

    CMethodArgumentList( const CLocation& _location ) : CVisitorTarget( _location ) {}

    void Add( const CMethodArgument* argument ) 
        { arguments.emplace_back( argument ); }

    const std::vector< std::unique_ptr<const CMethodArgument> >& MethodArguments() const { return arguments; }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::vector< std::unique_ptr<const CMethodArgument> > arguments;
};

}
