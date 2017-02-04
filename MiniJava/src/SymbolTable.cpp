#include <SymbolTable.h>

void PrintType( TTypeIdentifier type ) {
    std::string string;
    switch ( type ) {
        case TTypeIdentifier::Int:
        {
            string = "int";
            break;
        }
        case TTypeIdentifier::Boolean:
        {
            string = "boolean";
            break;
        }
        case TTypeIdentifier::IntArray:
        {
            string = "array";
            break;
        }
        case TTypeIdentifier::NotFound:
        {
            string = "notFound";
            break;
        }
        case TTypeIdentifier::ClassId:
        {
            string = "classId";
            break;
        }
        default:
        {
            string = "other";
            break;
        }
    }
    std::cout << string << std::endl;
}

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

std::shared_ptr<const CMethodDefinition> CSymbolTable::SearchClassHierarchyForMethod( const std::string& methodName,
        std::shared_ptr<const CClassDefinition> baseClass ) const {
    std::shared_ptr<const CMethodDefinition> methodDefinition = nullptr;
    while ( baseClass != nullptr ) {
        methodDefinition = baseClass->GetMethodDefinition( methodName );
        if ( methodDefinition != nullptr ) {
            break;
        }
        baseClass = baseClass->HasParent() ? GetClassDefinition( baseClass->GetParentName() ) : nullptr;
    }
    return methodDefinition;
}

CTypeIdentifier CSymbolTable::SearchClassHierarchyForField( const std::string& fieldName,
        std::shared_ptr<const CClassDefinition> baseClass ) const {
    CTypeIdentifier fieldType( TTypeIdentifier::NotFound );
    while ( baseClass != nullptr ) {
        fieldType = baseClass->GetFieldType( fieldName );
        if ( fieldType.Type() != TTypeIdentifier::NotFound ) {
            break;
        }
        baseClass = baseClass->HasParent() ? GetClassDefinition( baseClass->GetParentName() ) : nullptr;
    }
    return fieldType;
}

//////////////////////////////////////////

const std::string& CClassDefinition::ClassName() const {
    return className;
}

std::shared_ptr<const CMethodDefinition> CClassDefinition::GetMethodDefinition( const std::string& name ) const {
    auto result = methods->find( name );
    if ( result != methods->end() ) {
        return result->second;
    } else {
        return nullptr;
    }
}

CTypeIdentifier CClassDefinition::GetFieldType( const std::string& name ) const {
    auto result = fields->find( name );
    if ( result != fields->end() ) {
        return result->second;
    } else {
        return CTypeIdentifier( TTypeIdentifier::NotFound );
    }
}

const std::unordered_map<std::string, CTypeIdentifier>& CClassDefinition::Fields() const {
    return *fields;
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

CTypeIdentifier CMethodDefinition::GetVariableType( const std::string& name ) const {
    CTypeIdentifier type = GetLocalVariableType( name );
    if ( type.Type() == TTypeIdentifier::NotFound ) {
        type = GetArgumentType( name );
    }
    return type;
}

int CMethodDefinition::GetArgumentsNumber() const {
	return sortedArgumentsTypes->size();
}

std::shared_ptr<std::unordered_map<std::string, CTypeIdentifier>> CMethodDefinition::LocalVariables() const {
    return localVariableTypes;
}
