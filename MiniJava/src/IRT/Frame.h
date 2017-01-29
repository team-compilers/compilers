#pragma once

#include <unordered_map>
#include <vector>

#include <IRT/nodes/Expression.h>
#include <IRT/Temporary.h>
#include <IRT/Label.h>

namespace IRTree {

class IAddress {
public:
    virtual ~IAddress() {}
    virtual const CExpression* Expression( const CExpression* framePointer ) const = 0;
};

/**
Indicates a memory location at offset X from the frame pointer.
*/
class CAddressInFrame : public IAddress {
public:
    CAddressInFrame( int _offset ) : offset( _offset ) {}

    virtual ~CAddressInFrame() {}
    virtual const CExpression* Expression( const CExpression* framePointer ) const override;
protected:
    int offset;
};

class CAddressOfField : public CAddressInFrame {
public:
    CAddressOfField( int _offset ) : CAddressInFrame( _offset ) {}
    const CExpression* Expression( const CExpression* thisPointer ) const override;
};

/**
CAddressInRegister (T84) indicates that it will be held in "register" T84
*/
class CAddressInRegister : public IAddress {
public:
    CAddressInRegister( const CTemp& _temp ) : temp( _temp ) {}
    const CExpression* Expression( const CExpression* framePointer ) const override;
private:
    CTemp temp;
};

class CFrame {
public:
    CFrame( CLabel _name ) : name( _name ), freeOffset( 0 ) {}

    CTemp FramePointer() const;
    int WordSize() const;

    void AddArgument( const std::string& name );
    void AddLocalVariable( const std::string& name );
    void AddField( const std::string& name );

    const IAddress* Address( const std::string& varName ) const;

    const CExpression* ExternalCall( const std::string& functionName, const CExpressionList* args ) const;

private:
    int nextFreeAddressOffset();
    void addAddress( const std::string& name, const IAddress* address );

    CLabel name;
    std::unordered_map<std::string, std::unique_ptr<const IAddress>> addresses;

    CTemp framePointer;
    static const int wordSize;

    int freeOffset;
};


}