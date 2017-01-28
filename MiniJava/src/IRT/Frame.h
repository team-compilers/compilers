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
    const CExpression* Expression( const CExpression* framePointer ) const override;
private:
    int offset;
};

/**
CAddressInRegister (T84) indicates that it will be held in "register" T84
*/
class CAddressInRegister : public IAddress {
public:
    const CExpression* Expression( const CExpression* framePointer ) const override;
private:
    CTemp temp;
};

class CFrame {
public:
    CFrame( CLabel _name ) : name( _name ) {}

    CTemp FramePointer() const;
    int WordSize() const;

    void AddAddress( const std::string& varName, const IAddress* address );

    const CExpression* ExternalCall( const std::string& functionName, const CExpressionList* args ) const;

private:
    CLabel name;
    std::unordered_map<std::string, std::unique_ptr<const IAddress>> addresses;

    CTemp framePointer;
    int wordSize;

    int maxOffset;
};


}