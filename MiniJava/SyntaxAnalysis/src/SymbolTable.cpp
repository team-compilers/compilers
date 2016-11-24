#include <SymbolTable.h>

bool CSymbolTable::AddClassDefinition( const std::string& name, std::shared_ptr<const CClassDefinition> classDefinition ) {
    bool ok = classes.insert( std::make_pair( name, std::shared_ptr<const CClassDefinition>( classDefinition ) ) ).second;
    return ok;
}

std::shared_ptr<const CClassDefinition> CSymbolTable::GetClassDefinition( const std::string& name ) const {
	auto it = classes.find(name);
	if ( it == classes.end() ) {
		return nullptr;
	} else {
		return it->second;
	}
}

//////////////////////////////////////////

const std::string& CClassDefinition::ClassName() const {
    return className;
}

std::shared_ptr<const CMethodDefinition> CClassDefinition::GetMethodDefinition( const std::string& name ) const {
    auto result = methods->find( name );
    if( result != methods->end() ) {
        return result->second;
    } else {
        return nullptr;
    }
}

CTypeIdentifier CClassDefinition::GetFieldType( const std::string& name ) const {
    auto result = fields->find( name );
    if( result != fields->end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

////////////////////////////////////////////////////


CTypeIdentifier CMethodDefinition::GetLocalVariableType( const std::string& name ) const {
    auto result = localVariableTypes->find( name );
    if( result != localVariableTypes->end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

CTypeIdentifier CMethodDefinition::GetArgumentType( const std::string& name ) const {
    auto result = argumentTypes->find( name );
    if( result != argumentTypes->end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

CTypeIdentifier CMethodDefinition::GetArgumentType( int index ) const {
	if( index < sortedArgumentsTypes->size() ) {
		return (*sortedArgumentsTypes)[index];
	} else {
		return CTypeIdentifier( TTypeIdentifier::NotFound );
	}
}

int CMethodDefinition::GetArgumentsNumber() const {
	return sortedArgumentsTypes->size();
}