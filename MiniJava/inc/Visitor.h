// Author: Alexey Zhuravlev
// Description: Visitor interface

#pragma once

class CBinaryExpression;
class CBracketExpression;
class CNumberExpression;
class CLogicExpressin;
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
class CPublicAccessModifier;
class CPrivateAccessModifier;
class CMethodArgument;
class CMethodDeclaration;
class CIntTypeModifier;
class CBooleanTypeModifier;
class CIntArrayTypeModifier;
class CIdTypeModifier;
class CVarDeclaration;
class CClassDeclaration;
class CMainClass;
class CProgram;
class CStatementList;
class CExpressionList;
class CMethodDeclarationList;
class CClassDeclarationList;
class CMethodArgumentList;
class CVarDeclarationList;

class IVisitor {
public:
    virtual void Visit( CBinaryExpression* expression ) = 0;
    virtual void Visit( CBracketExpression* expression ) = 0; 
    virtual void Visit( CNumberExpression* expression ) = 0; 
    virtual void Visit( CLogicExpression* expression ) = 0; 
    virtual void Visit( CIdExpression* expression ) = 0;
    virtual void Visit( CLengthExpression* expression ) = 0; 
    virtual void Visit( CMethodExpression* expression ) = 0; 
    virtual void Visit( CThisExpression* expression ) = 0; 
    virtual void Visit( CNewArrayExpression* expression ) = 0; 
    virtual void Visit( CNewIdExpression* expression ) = 0; 
    virtual void Visit( CNegateExpression* expression ) = 0; 
    virtual void Visit( CAssignIdStatement* statement ) = 0; 
    virtual void Visit( CAssignIdWithIndexStatement* statement ) = 0; 
    virtual void Visit( CPrintStatement* statement ) = 0; 
    virtual void Visit( CConditionalStatement* statement ) = 0; 
    virtual void Visit( CWhileLoopStatement* statement ) = 0; 
    virtual void Visit( CBracesStatement* statement ) = 0; 
    virtual void Visit( CPrivateAccessModifier* modifier ) = 0;
    virtual void Visit( CPublicAccessModifier* modifier ) = 0;
    virtual void Visit( CMethodArgument* argument ) = 0;
    virtual void Visit( CMethodDeclaration* declaration ) = 0;
    virtual void Visit( CIntTypeModifier* intModifier ) = 0;
    virtual void Visit( CBooleanTypeModifier* booleanModifier ) = 0;
    virtual void Visit( CIntArrayTypeModifier* intArrayModifier ) = 0;
    virtual void Visit( CIdTypeModifier* idModifier ) = 0;
    virtual void Visit( CClassDeclaration* classDeclaration ) = 0;
    virtual void Visit( CMainClass* mainClass ) = 0;
    virtual void Visit( CVarDeclaration* varDeclaration ) = 0;
    virtual void Visit( CProgram* program ) = 0;
    virtual void Visit( CStatementList* list ) = 0;
    virtual void Visit( CExpressionList* list ) = 0;
    virtual void Visit( CMethodArgumentList* list ) = 0;
    virtual void Visit( CMethodDeclarationList* list ) = 0;
    virtual void Visit( CClassDeclarationList* list ) = 0;
    virtual void Visit( CMethodArgumentList* list ) = 0;
    virtual void Visit( CVarDeclarationList* list ) = 0;
};
