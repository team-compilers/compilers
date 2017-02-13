#include <IRT/Frame.h>
#include <cassert>

using namespace IRTree;

std::unique_ptr<const CExpression> CAddressInFrame::ToExpression( std::unique_ptr<const CExpression> framePointer ) const {
    return std::move( std::unique_ptr<const CBinaryExpression>(
        new CBinaryExpression(
            TOperatorType::OT_Plus,
            std::move( framePointer ),
            std::move( std::unique_ptr<const CExpression>( new CConstExpression( offset ) ) )
        )
    ) );
}

std::unique_ptr<const CExpression> CAddressOfField::ToExpression( std::unique_ptr<const CExpression> thisPointer ) const {
    return std::move( std::unique_ptr<const CBinaryExpression>(
        new CBinaryExpression(
            TOperatorType::OT_Plus,
            std::move( thisPointer ),
            std::move( std::unique_ptr<const CExpression>( new CConstExpression( offset ) ) )
        )
    ) );
}

std::unique_ptr<const CExpression> CAddressInRegister::ToExpression( std::unique_ptr<const CExpression> framePointer ) const {
    return std::move( std::unique_ptr<const CTempExpression>(
        new CTempExpression( temp )
    ) );
}


const int CFrame::wordSize = 4;
const std::string CFrame::thisName = "$this";
const std::string CFrame::returnName = "$return";

CTemp CFrame::FramePointer() const {
    return framePointer;
}

CTemp CFrame::ReturnValueTemp() const {
    return returnValueTemp;
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
    const CAddressInFrame* address = new CAddressInFrame( nextOffsetFromFramePointer() );
    addAddress( name, address );
}

void CFrame::AddField( const std::string& name ) {
    const CAddressOfField* address = new CAddressOfField( nextOffsetFromThis() );
    addAddress( name, address );
}

const IAddress* CFrame::GetAddress( const std::string& varName ) const {
    auto addressIt = addresses.find( varName );
    const IAddress* res = nullptr;
    if ( addressIt != addresses.end() ) {
        res = addressIt->second.get();
    }
    return res;
}

const IAddress* CFrame::GetThis() const {
    return GetAddress( thisName );
}

const IAddress* CFrame::GetReturn() const {
    return GetAddress( returnName );
}

std::unique_ptr<const CExpression> CFrame::ExternalCall( const std::string& functionName, std::unique_ptr<const CExpressionList> args ) const {
    return std::move( std::unique_ptr<const IRTree::CCallExpression>(
        new IRTree::CCallExpression(
            std::move( std::unique_ptr<const CNameExpression>(
                new CNameExpression( CLabel( functionName ) )
            ) ),
            std::move( args )
        )
    ) );
}

int CFrame::nextOffsetFromFramePointer() {
    int tmp = maxOffsetFramePointer;
    maxOffsetFramePointer += wordSize;
    return tmp;
}

int CFrame::nextOffsetFromThis() {
    int tmp = maxOffsetThis;
    maxOffsetThis += wordSize;
    return tmp;
}

void CFrame::addAddress( const std::string& name, const IAddress* address ) {
    auto result = addresses.emplace( name, std::unique_ptr<const IAddress>( address ) );
    // overwriting should not happen
    assert( result.second );
}
