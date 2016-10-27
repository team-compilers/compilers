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
};
