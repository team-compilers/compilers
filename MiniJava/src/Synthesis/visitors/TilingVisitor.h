#pragma once

#include <IRT/nodes/NodeNames.h>
#include <IRT/visitors/Visitor.h>

#include <IRT/nodes/Expression.h>
#include <IRT/nodes/ExpressionList.h>
#include <IRT/nodes/Statement.h>

#include <Synthesis/nodes/Patterns.h>

#include <vector>
#include <memory>

using namespace IRTree;

namespace Synthesis {

class CTilingVisitor : public IRTree::CVisitor {
public:
    CTilingVisitor( const IRTVT * _root, bool _verbose = false )
            : CVisitor( _verbose ), root( _root ) {
        instantiate_patterns<
            CTempPattern,
            CLabelPattern,
            CAddPattern,
            CSubPattern,
            CMultPattern,
            CDivPattern,
            CSubConstPattern,
            CAddConstLeftPattern,
            CAddConstRightPattern,
            CConstPattern,
            CLoadMemoryPattern,
            CLoadConstMemoryPattern,
            CLoadMemoryLeftOffsetPattern,
            CLoadMemoryRightOffsetPattern,
            CStoreMemoryPattern,
            CStoreConstMemoryPattern,
            CStoreMemoryLeftOffsetPattern,
            CStoreMemoryRightOffsetPattern,
            CStoreRegisterPattern,
            CCallFunctionPattern,
            CJumpPattern,
            CConditionalJumpPattern,
            CExpStatementPattern>();
    }
    ~CTilingVisitor() {}

    template<class First, class Second, class... Rest>
    void instantiate_patterns() {
        instantiate_patterns<First>();
        instantiate_patterns<Second, Rest...>();
    }

    template<class Pattern>
    void instantiate_patterns() {
        patterns.push_back(std::unique_ptr<CPattern>(new Pattern(&dynamic)));
    }

    void ApplyPatterns( const IRTVT* node ) {
        for(const auto& pattern : patterns) {
            pattern->Consume(node);
        }
    }

    // Visitors for different node types.
    void Visit( const CConstExpression* expression ) override;
    void Visit( const CNameExpression* expression ) override;
    void Visit( const IRTree::CTempExpression* expression ) override;
    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CMemExpression* expression ) override;
    void Visit( const CCallExpression* expression ) override;
    void Visit( const CEseqExpression* expression ) override;

    void Visit( const CExpStatement* statement ) override;
    void Visit( const CJumpConditionalStatement* statement ) override;
    void Visit( const CJumpStatement* statement ) override;
    void Visit( const CLabelStatement* statement ) override;
    void Visit( const CMoveStatement* statement ) override;
    void Visit( const CSeqStatement* statement ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;

    const CCommand* Result() const {
        return dynamic.at( root ).second.get();
    }

private:
    Dynamic dynamic;
    std::vector<std::unique_ptr<CPattern>> patterns;
    const IRTVT * root;
};

}
