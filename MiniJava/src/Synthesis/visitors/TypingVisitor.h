#pragma once

#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/Expression.h>

using namespace IRTree;

namespace Synthesis {

template<typename T>
class CTypingVisitor : public CVisitor {
public:
    CTypingVisitor( bool _verbose = false ) : CVisitor( _verbose ), node(nullptr) {}
    ~CTypingVisitor() {}

    // Visitors for different node types.
    void Visit( const CConstExpression* expression ) {};
    void Visit( const CNameExpression* expression ) {};
    void Visit( const CTempExpression* expression ) {};
    void Visit( const CBinaryExpression* expression ) {};
    void Visit( const CMemExpression* expression ) {};
    void Visit( const CCallExpression* expression ) {};
    void Visit( const CEseqExpression* expression ) {};

    void Visit( const CExpStatement* statement ) {};
    void Visit( const CJumpConditionalStatement* statement ) {};
    void Visit( const CJumpStatement* statement ) {};
    void Visit( const CLabelStatement* statement ) {};
    void Visit( const CMoveStatement* statement ) {};
    void Visit( const CSeqStatement* statement ) {};

    void Visit( const CExpressionList* list ) {};
    void Visit( const CStatementList* list ) {};

    const T* GetNode();
private:
    const T* node;
};

template<>
void CTypingVisitor<CConstExpression>::Visit( const CConstExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CNameExpression>::Visit( const CNameExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CTempExpression>::Visit( const CTempExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CBinaryExpression>::Visit( const CBinaryExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CMemExpression>::Visit( const CMemExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CCallExpression>::Visit( const CCallExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CEseqExpression>::Visit( const CEseqExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<CExpStatement>::Visit( const CExpStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<CJumpConditionalStatement>::Visit( const CJumpConditionalStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<CJumpStatement>::Visit( const CJumpStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<CLabelStatement>::Visit( const CLabelStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<CMoveStatement>::Visit( const CMoveStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<CSeqStatement>::Visit( const CSeqStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<CExpressionList>::Visit( const CExpressionList* list ) {
    node = list;
}

template<>
void CTypingVisitor<CStatementList>::Visit( const CStatementList* list ) {
    node = list;
}

template<typename T>
const T* CTypingVisitor<T>::GetNode() {
    return node;
}

}
