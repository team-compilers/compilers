#pragma once

#include <map>
#include <tuple>
#include <memory>

#include <IRT/nodes/VisitorTarget.h>
#include <Synthesis/nodes/VisitorTarget.h>
#include <Synthesis/nodes/Commands.h>

namespace Synthesis {

using IRTVT = IRTree::IVisitorTarget;
using Dynamic = std::map<const IRTVT*, std::tuple<int, std::unique_ptr<const CExpression>>>;

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

    template <typename Command>
    void ConsumeBinaryOperation( const IRTVT* node, IRTree::TOperatorType operation );

    template<typename T>
    ValidAndValue<const T*> GetTypedNode( const IRTVT* node );
    int GetDynamicPrice( const IRTVT* node );
    const CExpression* GetDynamicValue( const IRTVT* node );

    Dynamic* dynamic;
};

// Add Command
// r1 + r2
class CAddPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Sub command
// r1 - r2
class CSubPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Mult command
// r1 * r2
class CMultPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Div command
// r1 / r2
class CDivPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Sub const command
// r1 - c
class CSubConstPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Add const commands
// c + r1
class CAddConstLeftPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// r1 + c
class CAddConstRightPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// c
class CConstPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Load memory commands
// M[src]
class CLoadMemoryPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// M[C]
class CLoadConstMemoryPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// M[C+src]
class CLoadMemoryLeftOffsetPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// M[src+C]
class CLoadMemoryRightOffsetPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Store memory commands
// M[dst] = src
class CStoreMemoryPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// M[C] = src
class CStoreConstMemoryPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// M[C+dst] = src
class CStoreMemoryLeftOffsetPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// M[dst + C] = src
class CStoreMemoryRightOffsetPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

// Move commands
// M[dst] = M[src]
class CMoveMemoryPattern : public CPattern {
public:
    void Consume( const IRTVT* node ) override;
};

}
