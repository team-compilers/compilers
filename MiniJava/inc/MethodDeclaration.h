#pragma once

#include <VisitorTarget.h>
#include <Visitor.h>
#include <AccessModifier.h>
#include <TypeModifier.h>
#include <Expression.h>
#include <MethodArgumentList.h>
#include <VarDeclarationList.h>
#include <StatementList.h>

class CMethodDeclaration : public IVisitorTarget {
public:

    CMethodDeclaration( const IAccessModifier* _accessModifier, const ITypeModifier* _typeModifier,
            const CIdExpression* _methodId, const CMethodArgumentList* _methodArguments,
            const CVarDeclarationList* _varDeclarations, const CStatementList* _statements,
            const IExpression* _returnExpression )
        : accessModifier( _accessModifier ),
          typeModifier( _typeModifier ),
          methodArguments( _methodArguments ),
          varDeclarations( _varDeclarations ),
          statements( _statements ),
          returnExpression( _returnExpression ) {}

    const IAccessModifier* AccessModifier() const { return accessModifier; }
    const ITypeModifier* TypeModifier() const { return typeModifier; }
    const CIdExpression* MethodId() const { return methodId; }
    const CMethodArgumentList* MethodArguments() const { return methodArguments; }
    const CStatementList* Statements() const { return statements; }
    const IExpression* ReturnExpression() const { return returnExpression; }

    void Accept( IVisitor* visitor ) override { visitor->Visit( this ); }

private:
    std::unique_ptr<const IAccessModifier> accessModifier;
    std::unique_ptr<const ITypeModifier> typeModifier;
    std::unique_ptr<const CIdExpression> methodId;
    std::unique_ptr<const CMethodArgumentList> methodArguments;
    std::unique_ptr<const CVarDeclarationList> varDeclarations;
    std::unique_ptr<const CStatementList> statements;
    std::unique_ptr<const IExpression> returnExpression;
};