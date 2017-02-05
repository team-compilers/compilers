#include <AST/visitors/DummyVisitor.h>

using namespace ASTree;
/*__________ Access Modifiers __________*/

void CDummyVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName, modifier->Location() );

    // write your code here

    onNodeExit( nodeName, modifier->Location() );
}

void CDummyVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName, modifier->Location() );

    // write your code here

    onNodeExit( nodeName, modifier->Location() );
}

/*__________ Expressions __________*/

void CDummyVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ID );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_METHOD );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_THIS );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

void CDummyVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName, expression->Location() );

    // write your code here

    onNodeExit( nodeName, expression->Location() );
}

/*__________ Statements __________*/

void CDummyVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName, statement->Location() );

    // write your code here

    onNodeExit( nodeName, statement->Location() );
}

void CDummyVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName, statement->Location() );

    // write your code here

    onNodeExit( nodeName, statement->Location() );
}

void CDummyVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_PRINT );
    onNodeEnter( nodeName, statement->Location() );

    // write your code here

    onNodeExit( nodeName, statement->Location() );
}

void CDummyVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName, statement->Location() );

    // write your code here

    onNodeExit( nodeName, statement->Location() );
}

void CDummyVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName, statement->Location() );

    // write your code here

    onNodeExit( nodeName, statement->Location() );
}

void CDummyVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_BRACES );
    onNodeEnter( nodeName, statement->Location() );

    // write your code here

    onNodeExit( nodeName, statement->Location() );
}

/*__________ Type Modifiers __________*/

void CDummyVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName, typeModifier->Location() );

    // write your code here

    onNodeExit( nodeName, typeModifier->Location() );
}

void CDummyVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName, typeModifier->Location() );

    // write your code here

    onNodeExit( nodeName, typeModifier->Location() );
}

void CDummyVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName, typeModifier->Location() );

    // write your code here

    onNodeExit( nodeName, typeModifier->Location() );
}

void CDummyVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName, typeModifier->Location() );

    // write your code here

    onNodeExit( nodeName, typeModifier->Location() );
}

/*__________ Other (except lists) __________*/

void CDummyVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    // write your code here

    onNodeExit( nodeName, declaration->Location() );
}

void CDummyVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG );
    onNodeEnter( nodeName, argument->Location() );

    // write your code here

    onNodeExit( nodeName, argument->Location() );
}

void CDummyVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    // write your code here

    onNodeExit( nodeName, declaration->Location() );
}

void CDummyVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName, mainClass->Location() );

    // write your code here

    onNodeExit( nodeName, mainClass->Location() );
}

void CDummyVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    // write your code here

    onNodeExit( nodeName, declaration->Location() );
}

void CDummyVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CNodeNames::PROGRAM );
    onNodeEnter( nodeName, program->Location() );

    // write your code here

    onNodeExit( nodeName, program->Location() );
}

/*__________  Lists __________*/

void CDummyVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName, list->Location() );

    // write your code here

    onNodeExit( nodeName, list->Location() );
}

void CDummyVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName, list->Location() );

    // write your code here

    onNodeExit( nodeName, list->Location() );
}

void CDummyVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    // write your code here

    onNodeExit( nodeName, list->Location() );
}

void CDummyVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName, list->Location() );

    // write your code here

    onNodeExit( nodeName, list->Location() );
}

void CDummyVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    // write your code here

    onNodeExit( nodeName, list->Location() );
}

void CDummyVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    // write your code here

    onNodeExit( nodeName, list->Location() );
}
