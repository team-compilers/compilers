// Description: SymbolTableBuilderVisitor

#pragma once

#include <iostream> // for verbose output
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <AST/nodes/NodeNames.h>
#include <SymbolTable.h>
#include <CompilationError.h>
#include <AST/visitors/Visitor.h>

#include <AST/nodes/AccessModifier.h>
#include <AST/nodes/Expression.h>
#include <AST/nodes/ExpressionList.h>
#include <AST/nodes/Statement.h>
#include <AST/nodes/StatementList.h>
#include <AST/nodes/TypeModifier.h>
#include <AST/nodes/VarDeclaration.h>
#include <AST/nodes/VarDeclarationList.h>
#include <AST/nodes/MethodArgument.h>
#include <AST/nodes/MethodArgumentList.h>
#include <AST/nodes/MethodDeclaration.h>
#include <AST/nodes/MethodDeclarationList.h>
#include <AST/nodes/MainClass.h>
#include <AST/nodes/ClassDeclaration.h>
#include <AST/nodes/ClassDeclarationList.h>
#include <AST/nodes/Program.h>

namespace ASTree {

using MethodNameToDefinitionMap = std::unordered_map<std::string, std::shared_ptr<const CMethodDefinition>>;
using VarNameToTypeMap = std::unordered_map<std::string, CTypeIdentifier>;
using ArgumentsTypesList = std::vector<CTypeIdentifier>;

class CSymbolTableBuilderVisitor : public CVisitor {
public:
    CSymbolTableBuilderVisitor( bool _verbose = false ) 
        : CVisitor( _verbose ),
          lastMethodDefinition( nullptr ), lastClassDefinition(nullptr),
          methodDefinitions( nullptr ), table( new CSymbolTable() ),
          errors( new std::vector<CCompilationError>() ) {}

    ~CSymbolTableBuilderVisitor() {}

    const CSymbolTable* SymbolTable() const;
    const std::vector<CCompilationError>* Errors() const;

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
    std::shared_ptr<ArgumentsTypesList> sortedArgumentsTypes;

    std::shared_ptr<MethodNameToDefinitionMap> methodDefinitions;
    
    CSymbolTable* table;
    std::vector<CCompilationError>* errors;
};

}
