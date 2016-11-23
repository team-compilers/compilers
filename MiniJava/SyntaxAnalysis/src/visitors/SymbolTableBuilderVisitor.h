// Description: SymbolTableBuilderVisitor

#pragma once

#include <iostream> // for verbose output
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <AstNodeNames.h>
#include <SymbolTable.h>
#include <CompilationError.h>
#include <Visitor.h>

#include <AccessModifier.h>
#include <Expression.h>
#include <ExpressionList.h>
#include <Statement.h>
#include <StatementList.h>
#include <TypeModifier.h>
#include <VarDeclaration.h>
#include <VarDeclarationList.h>
#include <MethodArgument.h>
#include <MethodArgumentList.h>
#include <MethodDeclaration.h>
#include <MethodDeclarationList.h>
#include <MainClass.h>
#include <ClassDeclaration.h>
#include <ClassDeclarationList.h>
#include <Program.h>

using MethodNameToDefinitionMap = std::unordered_map<std::string, std::shared_ptr<const CMethodDefinition>>;
using VarNameToTypeMap = std::unordered_map<std::string, CTypeIdentifier>;

class CSymbolTableBuilderVisitor : public CVisitor {
public:
    CSymbolTableBuilderVisitor( bool _verbose = false ) 
        : CVisitor( _verbose ),
          lastMethodDefinition( nullptr ), lastClassDefinition(nullptr),
          methodDefinitions( nullptr ), table( new CSymbolTable() ),
          errors( new std::vector<CCompilationError>() ) {}

    ~CSymbolTableBuilderVisitor() {}

    std::shared_ptr<const CSymbolTable> SymbolTable() const;
    std::shared_ptr<const std::vector<CCompilationError>> Errors() const;

    // Visitors for different node types
    void Visit( const CPublicAccessModifier* modifier ) override;
    void Visit( const CPrivateAccessModifier* modifier ) override;

    void Visit( const CBinaryExpression* expression ) override;
    void Visit( const CBracketExpression* expression ) override;
    void Visit( const CNumberExpression* expression ) override;
    void Visit( const CLogicExpression* expression ) override;
    void Visit( const CIdExpression* expression ) override;
    void Visit( const CLengthExpression* expression ) override;
    void Visit( const CMethodExpression* expression ) override;
    void Visit( const CThisExpression* expression ) override;
    void Visit( const CNewArrayExpression* expression ) override;
    void Visit( const CNewIdExpression* expression ) override;
    void Visit( const CNegateExpression* expression ) override;

    void Visit( const CAssignIdStatement* statement ) override;
    void Visit( const CAssignIdWithIndexStatement* statement ) override;
    void Visit( const CPrintStatement* statement ) override;
    void Visit( const CConditionalStatement* statement ) override;
    void Visit( const CWhileLoopStatement* statement ) override;
    void Visit( const CBracesStatement* statement ) override;

    void Visit( const CIntTypeModifier* typeModifier ) override;
    void Visit( const CBooleanTypeModifier* typeModifier ) override;
    void Visit( const CIntArrayTypeModifier* typeModifier ) override;
    void Visit( const CIdTypeModifier* typeModifier ) override;

    void Visit( const CVarDeclaration* declaration ) override;
    void Visit( const CMethodArgument* argument ) override;
    void Visit( const CMethodDeclaration* declaration ) override;
    void Visit( const CMainClass* mainClass ) override;
    void Visit( const CClassDeclaration* declaration ) override;
    void Visit( const CProgram* program ) override;

    void Visit( const CExpressionList* list ) override;
    void Visit( const CStatementList* list ) override;
    void Visit( const CVarDeclarationList* list ) override;
    void Visit( const CMethodArgumentList* list ) override;
    void Visit( const CMethodDeclarationList* list ) override;
    void Visit( const CClassDeclarationList* list ) override;

private:
    CTypeIdentifier lastType;
    TAccessModifier lastAccessModifier;
    std::vector<std::string> lastId;
    
    std::shared_ptr<const CMethodDefinition> lastMethodDefinition;
    std::shared_ptr<const CClassDefinition> lastClassDefinition;
    
    std::vector<std::shared_ptr<VarNameToTypeMap>> localVariableTypes;
    std::shared_ptr<MethodNameToDefinitionMap> methodDefinitions;
    
    std::shared_ptr<CSymbolTable> table;

    std::shared_ptr<std::vector<CCompilationError>> errors;
};
