#include <Synthesis/visitors/CommandEmitterVisitor.h>
#include <Synthesis/AssemblyCommand.h>
#include <Synthesis/nodes/Commands.h>
#include <string>
#include <cassert>

using namespace Synthesis;
using LOT = IRTree::TLogicOperatorType;

int CAssemblyCommand::registerCounter = 0;

CCommandEmitterVisitor::CCommandEmitterVisitor() :
    CVisitor( false )
{
    tempToRegister["$fp"] = "EBP";
}

void CCommandEmitterVisitor::Visit( const CExpStatementCommand* statement )  {
    statement->Expression()->Accept( this );
}

void CCommandEmitterVisitor::Visit( const CConditionalJumpCommand* command ) {
    const CExpression* left = command->LeftPart();
    const CExpression* right = command->RightPart();

    left->Accept( this );
    std::string leftRegister = lastRegisterValue;
    right->Accept( this );
    std::string rightRegister = lastRegisterValue;

    auto cmp = command->Cmp();
    std::string ret;
    if (cmp == LOT::LOT_EQ) {
        ret = "JE";
    } else if (cmp == LOT::LOT_NE) {
        ret = "JNE";
    } else if (cmp == LOT::LOT_LT) {
        ret = "JL";
    } else if (cmp == LOT::LOT_GT) {
        ret = "JG";
    } else if (cmp == LOT::LOT_LE) {
        ret = "JLE";
    } else if (cmp == LOT::LOT_GE) {
        ret = "JGE";
    } else {
        assert( false );
    }

    code.push_back( CAssemblyCommand( ret + " " + command->PositiveLabelName(), { leftRegister, rightRegister } ) );
}

void CCommandEmitterVisitor::Visit( const CJumpCommand* command ) {
    code.push_back( CAssemblyCommand( "JMP " + command->LabelName(), {} ) );
}

void CCommandEmitterVisitor::Visit( const CCallFunctionCommand* command ) {
    const std::vector<const CExpression*>& arguments = command->Arguments();

    for(auto it = arguments.rbegin(); it != arguments.rend(); ++it) {
        (*it)->Accept( this );
        code.push_back( CAssemblyCommand( "PUSH", { lastRegisterValue } ));
    }

    code.push_back( CAssemblyCommand( "CALL " + command->Function(), {} ) );
}

void CCommandEmitterVisitor::Visit( const CLabelDeclarationCommand* command ) {
    code.push_back( CAssemblyCommand( command->Name() + ":", {} ) );
}

void CCommandEmitterVisitor::Visit( const CMoveRegisterCommand* command ) {
    const CExpression* source = command->Source();
    const CExpression* destination = command->Destination();

    source->Accept( this );
    std::string sourceRegister = lastRegisterValue;
    destination->Accept( this );
    std::string destinationRegister = lastRegisterValue;

    code.push_back( CAssemblyCommand( "MOV " + destinationRegister + "," + sourceRegister, 
        {sourceRegister, destinationRegister} ) );
}

void CCommandEmitterVisitor::Visit( const CTempExpression* expression ) {
    std::string tempValue = expression->Name();

    auto pos = tempToRegister.find( tempValue );
    if( pos == tempToRegister.end() ) {
        tempToRegister[tempValue] = CAssemblyCommand::NewRegister();
    }

    lastRegisterValue = tempToRegister[tempValue];
}

void CCommandEmitterVisitor::Visit( const CAddCommand* expression ) {
    const CExpression* leftOperand = expression->LeftOperand();
    const CExpression* rightOperand = expression->RightOperand();

    leftOperand->Accept( this );
    std::string leftRegister = lastRegisterValue;
    rightOperand->Accept( this );
    std::string rightRegister = lastRegisterValue;

    std::string resultRegister = CAssemblyCommand::NewRegister();

    code.push_back( CAssemblyCommand( "MOV " + resultRegister + "," + leftRegister, 
        { leftRegister, resultRegister } ) );
    code.push_back( CAssemblyCommand( "ADD " + resultRegister + "," + rightRegister,
        { resultRegister, rightRegister } ) );

    lastRegisterValue = resultRegister;
}

void CCommandEmitterVisitor::Visit( const CAddConstCommand* passedExpression ) {
    const CExpression* expression = passedExpression->Expression();
    const int constant = passedExpression->Constant();

    expression->Accept( this );
    std::string expressionRegister = lastRegisterValue;

    std::string resultRegister = CAssemblyCommand::NewRegister();

    code.push_back( CAssemblyCommand( "MOV " + resultRegister + "," + expressionRegister,
        { expressionRegister, resultRegister } ) );
    code.push_back( CAssemblyCommand( "ADD " + resultRegister + "," + std::to_string( constant ) ,
        { resultRegister } ) );

    lastRegisterValue = resultRegister;
}

void CCommandEmitterVisitor::Visit( const CSubCommand* expression ) {
    const CExpression* leftOperand = expression->LeftOperand();
    const CExpression* rightOperand = expression->RightOperand();

    leftOperand->Accept( this );
    std::string leftRegister = lastRegisterValue;
    rightOperand->Accept( this );
    std::string rightRegister = lastRegisterValue;

    std::string resultRegister = CAssemblyCommand::NewRegister();

    code.push_back( CAssemblyCommand( "MOV " + resultRegister + "," + leftRegister, 
        { leftRegister, resultRegister } ) );
    code.push_back( CAssemblyCommand( "SUB " + resultRegister + "," + rightRegister,
        { resultRegister, rightRegister } ) );

    lastRegisterValue = resultRegister;
}

void CCommandEmitterVisitor::Visit( const CSubConstCommand* passedExpression ) {
    const CExpression* expression = passedExpression->Expression();
    const int constant = passedExpression->Constant();

    expression->Accept( this );
    std::string expressionRegister = lastRegisterValue;

    std::string resultRegister = CAssemblyCommand::NewRegister();

    code.push_back( CAssemblyCommand( "MOV " + resultRegister + "," + expressionRegister, 
        { expressionRegister, resultRegister } ) );
    code.push_back( CAssemblyCommand( "SUB " + resultRegister + "," + std::to_string( constant ),
        { resultRegister } ) );

    lastRegisterValue = resultRegister;
}

void CCommandEmitterVisitor::Visit( const CMultCommand* expression ) {
    const CExpression* leftOperand = expression->LeftOperand();
    const CExpression* rightOperand = expression->RightOperand();

    leftOperand->Accept( this );
    std::string leftRegister = lastRegisterValue;
    rightOperand->Accept( this );
    std::string rightRegister = lastRegisterValue;

    std::string resultRegister = CAssemblyCommand::NewRegister();

    // http://www.felixcloutier.com/x86/MUL.html
    code.push_back( CAssemblyCommand( "MOV AL," + leftRegister,
        { leftRegister } ) );
    code.push_back( CAssemblyCommand( "DIV " + rightRegister,
        { rightRegister } ) );
    code.push_back( CAssemblyCommand( "MOV " + resultRegister + ",AX",
        { resultRegister } ) );

    lastRegisterValue = resultRegister;
}

void CCommandEmitterVisitor::Visit( const CDivCommand* expression ) {
    const CExpression* leftOperand = expression->LeftOperand();
    const CExpression* rightOperand = expression->RightOperand();

    leftOperand->Accept( this );
    std::string leftRegister = lastRegisterValue;
    rightOperand->Accept( this );
    std::string rightRegister = lastRegisterValue;

    std::string resultRegister = CAssemblyCommand::NewRegister();

    // http://www.felixcloutier.com/x86/DIV.html
    code.push_back( CAssemblyCommand( "MOV AX," + leftRegister, { leftRegister } ) );
    code.push_back( CAssemblyCommand( "DIV " + rightRegister, { rightRegister } ) );
    code.push_back( CAssemblyCommand( "MOV " + resultRegister + ",AL",
        { resultRegister } ) );

    lastRegisterValue = resultRegister;
}

void CCommandEmitterVisitor::Visit( const CNullExpression* expression ) {
    std::string reg = CAssemblyCommand::NewRegister();

    code.push_back( CAssemblyCommand( "MOV " + reg + ",0", {reg} ) );
    
    lastRegisterValue = reg;
}

void CCommandEmitterVisitor::Visit( const CLoadCommand* expression ) {
    const int offset = expression->Offset();
    const CExpression* expr = expression->Source();

    expr->Accept(this);
    std::string lastRegister = lastRegisterValue;

    std::string addressRegister = CAssemblyCommand::NewRegister();
    code.push_back( CAssemblyCommand( "MOV " + addressRegister + "," + lastRegister, {addressRegister, lastRegister} ) );
    code.push_back( CAssemblyCommand( "ADD " + addressRegister + "," + std::to_string(offset), {addressRegister} ) );

    std::string targetRegister = lastRegisterValue;

    code.push_back( CAssemblyCommand( "MOV " + targetRegister + "," + "[" + addressRegister + "]", {targetRegister, addressRegister} ) );
    lastRegisterValue = targetRegister;
}

void CCommandEmitterVisitor::Visit( const CStoreCommand* expression ) {
    const int offset = expression->Offset();
    const CExpression* source = expression->Source();
    const CExpression* destination = expression->Destination();


    destination->Accept(this);
    std::string destinationRegister = lastRegisterValue;
    source->Accept(this);
    std::string sourceRegister = lastRegisterValue;

    std::string addressRegister = CAssemblyCommand::NewRegister();
    code.push_back( CAssemblyCommand( "MOV " + addressRegister + "," + destinationRegister, {addressRegister, destinationRegister} ) );
    code.push_back( CAssemblyCommand( "ADD " + addressRegister + "," + std::to_string(offset), {addressRegister} ) );

    code.push_back( CAssemblyCommand( "MOV [" + destinationRegister + "]," + sourceRegister, {destinationRegister, sourceRegister} ) );
}
