#include <SymbolTable.h>

bool CSymbolTable::AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition ) {
    bool ok = classes.insert({name, classDefinition}).second;
    return ok;
}

//////////////////////////////////////////

const std::string& CSymbolTable::GetClassName() const {
    return className;
}

const CMethodDefinition* CClassDefinition::GetMethodDefinition( const std::string& name ) const {
    auto result = methods.find( name );
    if( result != methods.end() ) {
        return *result;
    } else {
        return 0;
    }
}

const CVariableDefinition* CClassDefinition::GetFieldDefinition( const std::string& name ) const {
    auto result = fields.find( name );
    if( result != fields.end() ) {
        return *result;
    } else {
        return 0;
    }
}

////////////////////////////////////////////////////


bool CMethodDefinition::AddLocalVariable( cosnt std::string& name, CTypeIdentifier type )
{
    bool ok = localVariableTypes.insert({name, type}).second;
    return ok;
}

CTypeIdentifier CMethodDefinition::GetLocalVariable( const std::string& name ) const {
    auto result = localVariableTypes.find( name );
    if( result != localVariableTypes.end() ) {
        return *result;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

CTypeIdentifier CMethodDefinition::GetArgumentType( const std::string& name ) const {
    auto result = localVariableTypes.find( name );
    if( result != localVariableTypes.end() ) {
        return *result;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}
