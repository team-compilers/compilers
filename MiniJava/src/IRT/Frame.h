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
    virtual std::unique_ptr<const CExpression> ToExpression() const = 0;
};

/**
Indicates a memory location at offset X from the frame pointer.
*/
class CAddressInFrame : public IAddress {
public:
    CAddressInFrame( const IAddress* _frameAddress, int _offset )
        : frameAddress( _frameAddress ), offset( _offset ) {}

    virtual ~CAddressInFrame() {}
    virtual std::unique_ptr<const CExpression> ToExpression() const override;
private:
    const IAddress* frameAddress;
    int offset;
};

class CAddressOfField : public IAddress {
public:
    CAddressOfField( const IAddress* _thisAddress, int _offset )
        : thisAddress( std::move( _thisAddress ) ), offset( _offset ) {}
    std::unique_ptr<const CExpression> ToExpression() const override;
private:
    const IAddress* thisAddress;
    int offset;
};

/**
CAddressInRegister (T84) indicates that it will be held in "register" T84
*/
class CAddressInRegister : public IAddress {
public:
    CAddressInRegister( const CTemp& _temp ) : temp( _temp ) {}
    std::unique_ptr<const CExpression> ToExpression() const override;
private:
    CTemp temp;
};

class CFrame {
public:
    // CFrame( CLabel _name ) : name( _name ), maxOffsetFramePointer( 0 ) {}
    CFrame( const std::string& _className, const std::string& _methodName );

    int GetWordSize() const;

    CLabel GetName() const;
    const std::string& GetClassName() const;
    const std::string& GetMethodName() const;

    // reserves place on stack for method arguments and locals (determines offsets for them)
    void AddThisAddress();
    void AddReturnAddress();
    void AddArgumentAddress( const std::string& name );
    void AddLocalAddress( const std::string& name );
    void AddFieldAddress( const std::string& name );

    const IAddress* GetFramePointerAddress() const;
    const IAddress* GetReturnValueAddress() const;
    const IAddress* GetThisAddress() const;
    const IAddress* GetReturnAddress() const;
    const IAddress* GetAddress( const std::string& varName ) const;

    std::unique_ptr<const CExpression> ExternalCall( const std::string& functionName, std::unique_ptr<const CExpressionList> args ) const;
private:
    int nextOffsetFromFramePointer();
    int nextOffsetFromThis();
    void addAddress( const std::string& name, const IAddress* address );

    std::string className;
    std::string methodName;
    CLabel name;
    std::unordered_map<std::string, std::unique_ptr<const IAddress>> addresses;

    static const int wordSize;
    static const std::string thisAddressName;
    static const std::string returnAddressName;
    static const std::string returnValueAddressName;
    static const std::string framePointerAddressName;

    int maxOffsetFramePointer;
    int maxOffsetThis;
};


}