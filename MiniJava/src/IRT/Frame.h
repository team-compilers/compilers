#pragma once;

#include <vector>

#include <IRT/nodes/Expression.h>
#include <IRT/Temporary.h>
#include <IRT/Label.h>

namespace IRTree {

class IAccess {
public:
    virtual const CExpression* Expression( const CExpression* framePointer ); 
};

/**
Indicates a memory location at offset X from the frame pointer.
*/
class CAccessInFrame : public IAccess {
private:
    int offset;
};

/**
CAccessInRegister (T84) indicates that it will be held in "register" T84
*/
class CAccessInRegister : public IAccess {
private:
    CTemp temp;
};

class CFrame {
public:
    CFrame() = default;

    NewFrame( CLabel name );

    CTemp FramePointer() const { return framePoiner; }
    int WordSize() const { return wordSize; }

    virtual IAccess AllocateLocal();
private:
    CLabel name;
    std::vector<IAccess> formals;

    CTemp framePointer;
    int wordSize;
};


}