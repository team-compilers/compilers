#include <IRT/Frame.h>
#include <cassert>

using namespace IRTree;

const CExpression* CAddressInFrame::ToExpression( const CExpression* framePointer ) const {
    return new CBinaryExpression(
        TOperatorType::OT_Plus,
        framePointer,
        new CConstExpression( offset )
    );
}

const CExpression* CAddressOfField::ToExpression( const CExpression* thisPointer ) const {
    return new CBinaryExpression(
        TOperatorType::OT_Plus,
        thisPointer,
        new CConstExpression( offset )
    );
}

const CExpression* CAddressInRegister::ToExpression( const CExpression* framePointer ) const {
    return new CTempExpression( temp );
}


const int CFrame::wordSize = 4;
const std::string CFrame::thisName = "$this";
const std::string CFrame::returnName = "$return";

CTemp CFrame::FramePointer() const {
    return framePointer;
}

int CFrame::WordSize() const {
    return wordSize;
}

CLabel CFrame::GetName() const {
    return name;
}

const std::string& CFrame::GetClassName() const {
    return className;
}

const std::string& CFrame::GetMethodName() const {
    return methodName;
}


void CFrame::AddThis() {
    AddArgument( thisName );
}

void CFrame::AddReturn() {
    AddArgument( returnName );
}

void CFrame::AddArgument( const std::string& name ) {
    AddLocal( name );
}

void CFrame::AddLocal( const std::string& name ) {
    const CAddressInFrame* address = new CAddressInFrame( nextFreeAddressOffset() );
    addAddress( name, address );
}

void CFrame::AddField( const std::string& name ) {
    const CAddressOfField* address = new CAddressOfField( nextFreeAddressOffset() );
    addAddress( name, address );
}

const IAddress* CFrame::GetAddress( const std::string& varName ) const {
    auto addressIt = addresses.find( varName );
    const IAddress* res;
    if ( addressIt == addresses.end() ) {
        res = nullptr;
    } else {
        res = (*addressIt).second.get();
    }
    return res;
}

const IAddress* CFrame::GetThis() const {
    GetAddress( thisName );
}

const IAddress* CFrame::GetReturn() const {
    GetAddress( returnName );
}

const CExpression* CFrame::ExternalCall( const std::string& functionName, const CExpressionList* args ) const {
    return new IRTree::CCallExpression(
        new CNameExpression( CLabel( functionName ) ),
        args
    );
}

int CFrame::nextFreeAddressOffset() {
    freeOffset += wordSize;
    return freeOffset;
}

void CFrame::addAddress( const std::string& name, const IAddress* address ) {
    auto result = addresses.emplace( name, std::unique_ptr<const IAddress>( address ) );
    // overwriting should not happen
    assert( result.second );
}
