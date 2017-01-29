#include <IRT/Frame.h>
#include <cassert>

using namespace IRTree;

const CExpression* CAddressInFrame::Expression( const CExpression* framePointer ) const {
    return new CBinaryExpression(
        TOperatorType::OT_Plus,
        framePointer,
        new CConstExpression( offset )
    );
}

const CExpression* CAddressOfField::Expression( const CExpression* thisPointer ) const {
    return new CBinaryExpression(
        TOperatorType::OT_Plus,
        thisPointer,
        new CConstExpression( offset )
    );
}

const CExpression* CAddressInRegister::Expression( const CExpression* framePointer ) const {
    return new CTempExpression( temp );
}


const int CFrame::wordSize = 4;

CTemp CFrame::FramePointer() const {
    return framePointer;
}

int CFrame::WordSize() const {
    return wordSize;
}

void CFrame::AddArgument( const std::string& name ) {
    AddLocalVariable( name );
}

void CFrame::AddLocalVariable( const std::string& name ) {
    const CAddressInFrame* address = new CAddressInFrame( nextFreeAddressOffset() );
    addAddress( name, address );
}

void CFrame::AddField( const std::string& name ) {
    const CAddressOfField* address = new CAddressOfField( nextFreeAddressOffset() );
    addAddress( name, address );
}

const IAddress* CFrame::Address( const std::string& varName ) const {
    return addresses.at( varName ).get();
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
