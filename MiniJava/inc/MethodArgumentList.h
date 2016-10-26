#pragma once

#include <VisitorTarget.h>
#include <Expression.h>
#include <TypeModifier.h>
#include <MethodArgument.h>
#include <vector>

class CMethodArgumentList : public IVisitorTarget {
public:

    CMethodArgumentList() {}

    void Add( const CMethodArgument* argument ) 
        { arguments.push_back(argument); }

private:
    std::vector< std::unique_ptr<const CMethodArgument> > arguments;
};
