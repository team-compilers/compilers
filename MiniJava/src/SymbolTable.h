// Author: Alexey Zhuravlev

#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

// Predeclarations and general enum and classes declarations

class CClassDefinition;
class CMethodDefinition;

enum class TAccessModifier {
    Public,
    Private
};

enum class TTypeIdentifier {
    Int,
    IntArray,
    Boolean,
    ClassId,

// Value to return if no valid variable type found
    NotFound
};

void PrintType( TTypeIdentifier type );

// Type identifier class for other methods
class CTypeIdentifier {
public:
    CTypeIdentifier() = default;
    CTypeIdentifier( TTypeIdentifier _type )
        : type( _type ) { assert( type != TTypeIdentifier::ClassId ); }

    explicit CTypeIdentifier( const std::string& _className )
        : type( TTypeIdentifier::ClassId ), className( _className ) {}

    TTypeIdentifier Type() const { return type; }
    const std::string& ClassName() const { return className; }

    bool operator==(const CTypeIdentifier& other) const {
    	if (type == other.type) {
    		if (type == TTypeIdentifier::ClassId) {
    			return className == other.className;
    		} else {
    			return true;
    		}
    	}
    	return false;
    }

    bool operator!=(const CTypeIdentifier& other) const {
    	return !((*this) == other);
    }

private:
    TTypeIdentifier type;
    // Valid if Type==TTypeIdentifier::ClassId
    std::string className;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class CSymbolTable {
public:
    CSymbolTable() {}

    // Add class definition. Return true on success. False if such class already exists
    bool AddClassDefinition( const std::string& name, std::shared_ptr<const CClassDefinition> classDefinition );
    // Get class definition by name. Returns zero pointer if it doesn't exist
    std::shared_ptr<const CClassDefinition> GetClassDefinition( const std::string& name ) const;

    std::shared_ptr<const CMethodDefinition> SearchClassHierarchyForMethod( const std::string& methodName, std::shared_ptr<const CClassDefinition> baseClass ) const;
    CTypeIdentifier SearchClassHierarchyForField( const std::string& fieldName, std::shared_ptr<const CClassDefinition> baseClass ) const;
private:
    std::unordered_map<std::string, std::shared_ptr<const CClassDefinition>> classes;
};

////////////////////////////////////////////////////////////////////////////////

class CClassDefinition {
public:
    using TNameToMethodDefinitionMap = std::unordered_map<std::string, std::shared_ptr<const CMethodDefinition>>;
    using TNameToFieldTypeMap = std::unordered_map<std::string, CTypeIdentifier>;

    // Create class defintion without parent
    CClassDefinition( const std::string& _className, std::shared_ptr<TNameToMethodDefinitionMap> _methods, 
            std::shared_ptr<TNameToFieldTypeMap> _fields )
        : className( _className ), hasParent( false ), methods( _methods ), fields( _fields ) {}

    CClassDefinition( const std::string& _className, const std::string& _parentName,
            std::shared_ptr<TNameToMethodDefinitionMap> _methods, std::shared_ptr<TNameToFieldTypeMap> _fields )
        : className( _className ), parentName( _parentName ), hasParent( true ), 
          methods( _methods ), fields( _fields ) {}

    ~CClassDefinition() = default;

    const std::string& ClassName() const;
    // Get method definition by name. Zero if not exists
    std::shared_ptr<const CMethodDefinition> GetMethodDefinition( const std::string& name ) const;

    // Get field definition by name. NotFound if not exists
    CTypeIdentifier GetFieldType( const std::string& name ) const;
    const std::unordered_map<std::string, CTypeIdentifier>& Fields() const;
    bool HasParent() const { return hasParent; }
    std::string GetParentName() const { return parentName; }
private:
    std::string className;
    std::shared_ptr<TNameToMethodDefinitionMap> methods;
    std::shared_ptr<TNameToFieldTypeMap> fields;
    bool hasParent;
    std::string parentName;
};

////////////////////////////////////////////////////////////////////////////////////

class CMethodDefinition {
public:
    using TNameToVarTypeMap = std::unordered_map<std::string, CTypeIdentifier>;
    using ArgumentsTypesList = std::vector<CTypeIdentifier>;
    CMethodDefinition( 
            TAccessModifier _accessModifier,
            const std::string& _methodName,
            CTypeIdentifier _returnType,
            std::shared_ptr<TNameToVarTypeMap> _argumentTypes,
            std::shared_ptr<ArgumentsTypesList> _sortedArgumentsTypes,
            std::shared_ptr<TNameToVarTypeMap> _localVariablesTypes
        ) : accessModifier( _accessModifier ),
            methodName( _methodName ),
            returnType( _returnType ),
            argumentTypes( _argumentTypes ),
            sortedArgumentsTypes(_sortedArgumentsTypes),
            localVariableTypes( _localVariablesTypes ) {}
    ~CMethodDefinition() {};

    TAccessModifier AccessModifier() const { return accessModifier; }
    const std::string& MethodName() const { return methodName; }
    CTypeIdentifier ReturnType() const { return returnType; }

	// Return NotFound in CTypeIdentifier if variable not found
    CTypeIdentifier GetLocalVariableType( const std::string& name ) const;
    CTypeIdentifier GetArgumentType( const std::string& name ) const;
    CTypeIdentifier GetArgumentType( int index ) const;
    CTypeIdentifier GetVariableType( const std::string& name ) const;
    int GetArgumentsNumber() const;

    std::shared_ptr<TNameToVarTypeMap> LocalVariables() const;

private:
    TAccessModifier accessModifier;
    std::string methodName;
    CTypeIdentifier returnType;
    std::shared_ptr<TNameToVarTypeMap> argumentTypes;
    std::shared_ptr<TNameToVarTypeMap> localVariableTypes;
    std::shared_ptr<ArgumentsTypesList> sortedArgumentsTypes;
};
