#pragma once

#include <memory>
#include <vector>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <Expression.h>
#include <MethodArgument.h>
#include <TypeModifier.h>

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