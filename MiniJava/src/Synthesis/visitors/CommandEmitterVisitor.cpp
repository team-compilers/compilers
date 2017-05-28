#include <Synthesis/visitors/CommandEmitterVisitor.h>
#include <Synthesis/AssemblyCommand.h>
#include <Synthesis/nodes/Commands.h>
#include <string>

using namespace Synthesis;

int CAssemblyCommand::registerCounter = 0;

void CCommandEmitterVisitor::Visit( const CConditionalJumpCommand* command ) 
{

}

void CCommandEmitterVisitor::Visit( const CJumpCommand* command ) 
{

}

void CCommandEmitterVisitor::Visit( const CCallFunctionCommand* command )
{

}

void CCommandEmitterVisitor::Visit( const CLabelDeclarationCommand* command )
{

}

void CCommandEmitterVisitor::Visit( const CMoveRegisterCommand* command ) 
{
    const CExpression* source = command->Source();
    const CExpression* destination = command->Destination();

    source->Accept( this );
    std::string sourceRegister = lastRegisterValue;
    destination->Accept( this );
    std::string destinationRegister = lastRegisterValue;

    code.push_back( CAssemblyCommand( "MOV " + destinationRegister + "," + sourceRegister, 
        {sourceRegister, destinationRegister} ) );
}

void CCommandEmitterVisitor::Visit( const CTempExpression* expression )
{
    std::string tempValue = expression->Name();

    auto pos = tempToRegister.find( tempValue );
    if( pos == tempToRegister.end() ) {
        tempToRegister[tempValue] = CAssemblyCommand::NewRegister();
    }

    lastRegisterValue = tempToRegister[tempValue];
}

void CCommandEmitterVisitor::Visit( const CAddCommand* expression ) 
{
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

void CCommandEmitterVisitor::Visit( const CAddConstCommand* passedExpression ) 
{
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

void CCommandEmitterVisitor::Visit( const CSubCommand* expression ) 
{
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

void CCommandEmitterVisitor::Visit( const CSubConstCommand* passedExpression )
{
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

void CCommandEmitterVisitor::Visit( const CMultCommand* expression ) 
{
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

void CCommandEmitterVisitor::Visit( const CDivCommand* expression ) 
{
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

void CCommandEmitterVisitor::Visit( const CNullExpression* expression ) 
{
    std::string reg = CAssemblyCommand::NewRegister();

    code.push_back( CAssemblyCommand( "MOV " + reg + ",0", {reg} ) );
    
    lastRegisterValue = reg;
}

void CCommandEmitterVisitor::Visit( const CLoadCommand* expression ) 
{

}

void CCommandEmitterVisitor::Visit( const CStoreCommand* expression ) 
{

}

void CCommandEmitterVisitor::Visit( const CMoveCommand* expression ) 
{

}