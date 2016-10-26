#pragma once

#include <VisitorTarget.h>
#include <Expression.h>
#include <TypeModifier.h>
#include <vector>

class CMethodArgument : public IVisitorTarget {
public:

    CMethodArgument( const ITypeModifier* _argumentType, const CIdExpression* _argumentId )
        : argumentType( _argumentType ),
          argumentId( _argumentId ) {}

    const ITypeModifier* ArgumentType() const { return argumentType; }
    const CIdExpression* ArgumentId() const { return arugmentId; }

private:
    std::unique_ptr<const ITypeModifier> argumentType;
    std::unique_ptr<const CIdExpression> argumentId;
};

//---------------------------------------------------------------------------------------//

class CMethodArgumentList : public IVisitorTarget {
public:

    CMethodArgumentList() {}

    void Add( const CMethodArgument* argument ) 
        { arguments.push_back(argument); }

private:
    std::vector< std::unique_ptr<const CMethodArgument> > arguments;
};
