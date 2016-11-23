#include <SymbolTable.h>

bool CSymbolTable::AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition ) {
    classes[name] = classDefinition;
    return true;
}

//////////////////////////////////////////

const CMethodDefinition* CClassDefinition::GetMethodDefinition( const std::string& name ) const {
    auto result = methods.find( name );
    if( result != methods.end() ) {
        return result->second.get();
    } else {
        return 0;
    }
}

CTypeIdentifier CClassDefinition::GetFieldType( const std::string& name ) const {
    auto result = fields.find( name );
    if( result != fields.end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

////////////////////////////////////////////////////


CTypeIdentifier CMethodDefinition::GetLocalVariableType( const std::string& name ) const {
    auto result = localVariableTypes.find( name );
    if( result != localVariableTypes.end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

CTypeIdentifier CMethodDefinition::GetArgumentType( const std::string& name ) const {
    auto result = localVariableTypes.find( name );
    if( result != localVariableTypes.end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}
