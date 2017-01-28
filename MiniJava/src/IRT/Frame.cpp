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

const CExpression* CAddressInRegister::Expression( const CExpression* framePointer ) const {
    return new CTempExpression( temp );
}

CTemp CFrame::FramePointer() const {
    return framePointer;
}

int CFrame::WordSize() const {
    return wordSize;
}

void CFrame::AddAddress( const std::string& varName, const IAddress* address ) {
    auto result = addresses.emplace( varName, std::unique_ptr<const IAddress>( address ) );
    // overwriting should not happen
    assert( result.second );
}

const CExpression* CFrame::ExternalCall( const std::string& functionName, const CExpressionList* args ) const {
    return new IRTree::CCallExpression(
        new CNameExpression( CLabel( functionName ) ),
        args
    );
}
