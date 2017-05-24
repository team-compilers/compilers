#include <Synthesis/visitors/CommandEmitterVisitor.h>
#include <Synthesis/AssemblyCommand.h>
#include <Synthesis/nodes/Commands.h>

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

    code.push_back( CAssemblyCommand( "MOV " + sourceRegister + "," + destinationRegister, 
        {sourceRegister, destinationRegister} ) );
}

void CCommandEmitterVisitor::Visit( const CTempExpression* expression )
{

}

void CCommandEmitterVisitor::Visit( const CAddCommand* expression ) 
{

}

void CCommandEmitterVisitor::Visit( const CAddConstCommand* expression ) 
{

}

void CCommandEmitterVisitor::Visit( const CSubCommand* expression ) 
{

}

void CCommandEmitterVisitor::Visit( const CSubConstCommand* expression )
{

}

void CCommandEmitterVisitor::Visit( const CMultCommand* expression ) 
{

}

void CCommandEmitterVisitor::Visit( const CDivCommand* expression ) 
{

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

void CCommandEmitterVisitor::Visit( const CNullExpression* expression ) 
{

}
