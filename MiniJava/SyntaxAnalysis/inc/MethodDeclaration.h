#pragma once

#include <memory>

#include <Visitor.h>
#include <VisitorTarget.h>

#include <AccessModifier.h>
#include <Expression.h>
#include <MethodArgumentList.h>
#include <StatementList.h>
#include <TypeModifier.h>
#include <VarDeclarationList.h>

class CMethodDeclaration : public IVisitorTarget {
public:

    CMethodDeclaration( const IAccessModifier* _accessModifier, const ITypeModifier* _typeModifier,
            const CIdExpression* _methodId, const CMethodArgumentList* _methodArguments,
            const CVarDeclarationList* _varDeclarations, const CStatementList* _statements,
            const IExpression* _returnExpression )
        : accessModifier( _accessModifier ),
          typeModifier( _typeModifier ),
          methodId( _methodId ),
          methodArguments( _methodArguments ),
          varDeclarations( _varDeclarations ),
          statements( _statements ),
          returnExpression( _returnExpression ) {}

    const IAccessModifier* AccessModifier() const { return accessModifier.get(); }
    const ITypeModifier* TypeModifier() const { return typeModifier.get(); }
    const CIdExpression* MethodId() const { return methodId.get(); }
    const CMethodArgumentList* MethodArguments() const { return methodArguments.get(); }
    const CVarDeclarationList* VarDeclarations() const { return varDeclarations.get(); }
    const CStatementList* Statements() const { return statements.get(); }
    const IExpression* ReturnExpression() const { return returnExpression.get(); }

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    std::unique_ptr<const IAccessModifier> accessModifier;
    std::unique_ptr<const ITypeModifier> typeModifier;
    std::unique_ptr<const CIdExpression> methodId;
    std::unique_ptr<const CMethodArgumentList> methodArguments;
    std::unique_ptr<const CVarDeclarationList> varDeclarations;
    std::unique_ptr<const CStatementList> statements;
    std::unique_ptr<const IExpression> returnExpression;
};
