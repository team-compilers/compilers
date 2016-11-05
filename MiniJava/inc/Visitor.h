// Author: Alexey Zhuravlev && Andrew Sautin
// Description: Visitor interface

#pragma once

class CPublicAccessModifier;
class CPrivateAccessModifier;

class CBinaryExpression;
class CBracketExpression;
class CNumberExpression;
class CLogicExpression;
class CIdExpression;
class CLengthExpression;
class CMethodExpression;
class CThisExpression;
class CNewArrayExpression;
class CNewIdExpression;
class CNegateExpression;

class CAssignIdStatement;
class CAssignIdWithIndexStatement;
class CPrintStatement;
class CConditionalStatement;
class CWhileLoopStatement;
class CBracesStatement;

class CIntTypeModifier;
class CBooleanTypeModifier;
class CIntArrayTypeModifier;
class CIdTypeModifier;

class CVarDeclaration;
class CMethodArgument;
class CMethodDeclaration;
class CMainClass;
class CClassDeclaration;
class CProgram;

class CExpressionList;
class CStatementList;
class CVarDeclarationList;
class CMethodArgumentList;
class CMethodDeclarationList;
class CClassDeclarationList;

class IVisitor {
public:
    virtual ~IVisitor() {}
    virtual void Visit( const CPublicAccessModifier* modifier ) = 0;
    virtual void Visit( const CPrivateAccessModifier* modifier ) = 0;

    virtual void Visit( const CBinaryExpression* expression ) = 0;
    virtual void Visit( const CBracketExpression* expression ) = 0;
    virtual void Visit( const CNumberExpression* expression ) = 0;
    virtual void Visit( const CLogicExpression* expression ) = 0;
    virtual void Visit( const CIdExpression* expression ) = 0;
    virtual void Visit( const CLengthExpression* expression ) = 0;
    virtual void Visit( const CMethodExpression* expression ) = 0;
    virtual void Visit( const CThisExpression* expression ) = 0;
    virtual void Visit( const CNewArrayExpression* expression ) = 0;
    virtual void Visit( const CNewIdExpression* expression ) = 0;
    virtual void Visit( const CNegateExpression* expression ) = 0;

    virtual void Visit( const CAssignIdStatement* statement ) = 0;
    virtual void Visit( const CAssignIdWithIndexStatement* statement ) = 0;
    virtual void Visit( const CPrintStatement* statement ) = 0;
    virtual void Visit( const CConditionalStatement* statement ) = 0;
    virtual void Visit( const CWhileLoopStatement* statement ) = 0;
    virtual void Visit( const CBracesStatement* statement ) = 0;

    virtual void Visit( const CIntTypeModifier* typeModifier ) = 0;
    virtual void Visit( const CBooleanTypeModifier* typeModifier ) = 0;
    virtual void Visit( const CIntArrayTypeModifier* typeModifier ) = 0;
    virtual void Visit( const CIdTypeModifier* typeModifier ) = 0;

    virtual void Visit( const CVarDeclaration* declaration ) = 0;
    virtual void Visit( const CMethodArgument* argument ) = 0;
    virtual void Visit( const CMethodDeclaration* declaration ) = 0;
    virtual void Visit( const CMainClass* mainClass ) = 0;
    virtual void Visit( const CClassDeclaration* declaration ) = 0;
    virtual void Visit( const CProgram* program ) = 0;

    virtual void Visit( const CExpressionList* list ) = 0;
    virtual void Visit( const CStatementList* list ) = 0;
    virtual void Visit( const CVarDeclarationList* list ) = 0;
    virtual void Visit( const CMethodArgumentList* list ) = 0;
    virtual void Visit( const CMethodDeclarationList* list ) = 0;
    virtual void Visit( const CClassDeclarationList* list ) = 0;
};
