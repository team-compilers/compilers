#pragma once

#include <IRT/visitors/Visitor.h>
#include <IRT/nodes/Expression.h>

using namespace IRTree;

namespace Synthesis {

template<typename T>
class CTypingVisitor : public IRTree::CVisitor {
public:
    CTypingVisitor( bool _verbose = false ) : CVisitor( _verbose ), node(nullptr) {}
    ~CTypingVisitor() {}

    // Visitors for different node types.
    void Visit( const IRTree::CConstExpression* expression ) {};
    void Visit( const IRTree::CNameExpression* expression ) {};
    void Visit( const IRTree::CTempExpression* expression ) {};
    void Visit( const IRTree::CBinaryExpression* expression ) {};
    void Visit( const IRTree::CMemExpression* expression ) {};
    void Visit( const IRTree::CCallExpression* expression ) {};
    void Visit( const IRTree::CEseqExpression* expression ) {};

    void Visit( const IRTree::CExpStatement* statement ) {};
    void Visit( const IRTree::CJumpConditionalStatement* statement ) {};
    void Visit( const IRTree::CJumpStatement* statement ) {};
    void Visit( const IRTree::CLabelStatement* statement ) {};
    void Visit( const IRTree::CMoveStatement* statement ) {};
    void Visit( const IRTree::CSeqStatement* statement ) {};

    void Visit( const IRTree::CExpressionList* list ) {};
    void Visit( const IRTree::CStatementList* list ) {};

    const T* GetNode();
private:
    const T* node;
};

template<>
void CTypingVisitor<IRTree::CConstExpression>::Visit( const IRTree::CConstExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CNameExpression>::Visit( const IRTree::CNameExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CTempExpression>::Visit( const IRTree::CTempExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CBinaryExpression>::Visit( const IRTree::CBinaryExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CMemExpression>::Visit( const IRTree::CMemExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CCallExpression>::Visit( const IRTree::CCallExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CEseqExpression>::Visit( const IRTree::CEseqExpression* expression ) {
    node = expression;
}

template<>
void CTypingVisitor<IRTree::CExpStatement>::Visit( const IRTree::CExpStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<IRTree::CJumpConditionalStatement>::Visit( 
        const IRTree::CJumpConditionalStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<IRTree::CJumpStatement>::Visit( const IRTree::CJumpStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<IRTree::CLabelStatement>::Visit( const IRTree::CLabelStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<IRTree::CMoveStatement>::Visit( const IRTree::CMoveStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<IRTree::CSeqStatement>::Visit( const IRTree::CSeqStatement* statement ) {
    node = statement;
}

template<>
void CTypingVisitor<IRTree::CExpressionList>::Visit( const IRTree::CExpressionList* list ) {
    node = list;
}

template<>
void CTypingVisitor<IRTree::CStatementList>::Visit( const IRTree::CStatementList* list ) {
    node = list;
}

template<typename T>
const T* CTypingVisitor<T>::GetNode() {
    return node;
}

}
