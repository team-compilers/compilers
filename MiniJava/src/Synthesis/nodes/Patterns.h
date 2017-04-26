#pragma once

#include <map>
#include <tuple>

#include <IRT/nodes/VisitorTarget.h>
#include <Synthesis/nodes/VisitorTarget.h>

namespace Synthesis {

using IRTVT = IRTree::IVisitorTarget;
using Dynamic = std::map<const IRTVT*, std::tuple<int, const IVisitorTarget*>>;

class CPattern {
public:
    CPattern( Dynamic* _dynamic ) : dynamic(_dynamic) {}
    virtual void Consume( const IRTVT* node ) = 0;

protected:
    template<typename T>
    class ValidAndValue {
    public:
        ValidAndValue( bool _valid, const T& _value ) : valid( _valid ), value( _value ) {}
        T operator->() const { return value; }
        T operator*() const { return value; }
        bool IsValid() const { return valid; }
    private:
        bool valid;
        T value;
    };

    template<typename T>
    ValidAndValue<const T*> GetTypedNode( const IRTVT* node );
    int GetDynamicPrice( const IRTVT* node );

    Dynamic* dynamic;
};

class CAddPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

}
