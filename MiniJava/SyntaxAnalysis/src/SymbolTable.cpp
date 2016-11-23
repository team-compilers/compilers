#include <SymbolTable.h>

bool CSymbolsTable::AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition ) {
    if( classes.count( name ) == 0 ) {
        return false;
    } else {
        classes[name] = classDefinition;
        return true;
    }
}

//////////////////////////////////////////


const CMethodDefinition* GetMethodDefinition( const std::string& name ) const {
    if( methods.count( name ) == 0 ) {
        return 0;
    } else {
        return methods[name];
    }
}

const CFieldDefinition* GetFieldDefinition( const std::string& name ) const {
    if( fields.count( name ) == 0 ) {
        return 0;
    } else {
        return fields[name];
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
    if( localVariableTypes.count( name ) == 0 ) {
        return false;
    } else {
        localVariableTypes[name] = type;
        return true;
    }
}

CTypeIdentifier CMethodDefinition::GetLocalVariable( const std::string& name ) const {
    if( localVariableTypes.count(name) == 0 ) {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    } else {
        return localVariableTypes[name];
    }
}

CTypeIdentifier CMethodDefinition::GetArgumentType( const std::string& name ) const {
    if( argumentTypes.count(name) == 0 ) {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    } else {
        return argumentTypes[name];
    }
}
