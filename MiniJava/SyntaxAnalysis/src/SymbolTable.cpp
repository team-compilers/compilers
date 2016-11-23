#include <SymbolTable.h>

bool CSymbolsTable::AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition ) {
    bool ok = clases.insert({name, classDefinition}).second;
    return ok;
}

//////////////////////////////////////////

const std::string& CSymbolTable::GetClassName() const {
    return className;
}

const CMethodDefinition* GetMethodDefinition( const std::string& name ) const {
    auto result = methods.find( name );
    if( result != methods.end() ) {
        return *result;
    } else {
        return 0;
    }
}

const CFieldDefinition* CSymbolTable::GetFieldDefinition( const std::string& name ) const {
    auto result = fields.find( name );
    if( result != fields.end() ) {
        return *result;
    } else {
        return 0;
    }
}

////////////////////////////////////////////////////


CMethodDefinition:CMethodDefinition( TAccessModifier _accessModifier,
    const std::string& _methodName, CTypeIdentifier _returnType,
    const std::vector<CVariableDefinition>& arguments )
    : accessModifier( _accessModifier ),
      methodName( _methodName),
      returnType( _returnType ) {
    for( unsigned int i = 0; i < arguments.size(); i++ ) {
        argumentTypes[arguments[i].Name()] = arguments[i].Type();
    }
}

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
