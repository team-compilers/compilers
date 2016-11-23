// Author: Alexey Zhuravlev

#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>
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

// Type identifier class for other methods
class CTypeIdentifier {
public:
    CTypeIdentifier( TTypeIdentifier _type )
        : type( _type ) { assert( type != TTypeIdentifier::ClassId ); }

    explicit CTypeIdentifier( const std::string& _className )
        : type( TTypeIdentifier::ClassId ), className( _className ) {}

    TTypeIdentifier Type() const { return type; }
    const std::string& ClassName() const { return className; }

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
    bool AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition );
    // Get class definition by name. Returns zero pointer if it doesn't exist
    const CClassDefinition* GetClassDefinition( const std::string& name ) const;

private:
    std::unordered_map<std::string, std::unique_ptr<CClassDefinition>> classes;
};

////////////////////////////////////////////////////////////////////////////////

class CClassDefinition {
public:
    typedef std::unordered_map<std::string, std::unique_ptr<CMethodDefinition> > TNameToMethodDefinitionMap;
    typedef std::unordered_map<std::string, CTypeIdentifier > TNameToFieldTypeMap;

    // Create class defintion without parent
    CClassDefinition( const std::string& _className, const TNameToMethodDefinitionMap& _methods, 
            const TNameToFieldTypeMap& _fields )
        : className( _className ), hasParent( false ), methods( _methods ), fields( _fields ) {}

    CClassDefinition( const std::string& _className, const std::string& _parentName,
            const TNameToMethodDefinitionMap& _methods, const TNameToFieldDefinitionMap& _fields )
        : className( _className ), parentName( _parentName ), hasParent( true ), 
          methods( _methods ), fields( _fields ) {}

    const std::string& ClassName() const;
    // Get method definition by name. Zero if not exists
    const CMethodDefinition* GetMethodDefinition( const std::string& name ) const;
    // Get field definition by name. Zero if not exists
    const CVariableDefinition* GetFieldDefinition( const std::string& name ) const;
private:
    std::string className;
    TNameToMethodDefinitionMap methods;
    TNameToFieldTypeMap fields;
    bool hasParent;
    std::string parentName;
};

////////////////////////////////////////////////////////////////////////////////////

class CMethodDefinition {
public:
    typedef std::unordered_map<std::string, CTypeIdentifier> TNameToIdentifierMapping;

    CMethodDefinition( TAccessModifier _accessModifier, const std::string& _methodName, 
            CTypeIdentifier _returnType, const TNameToIdentifierMapping& _argumentTypes,
            const TNameToIdentifierMapping& _localVariablesTypes )
        : accessModifier( _accessModifier ),
          methodName( _methodName ),
          returnType( _returnType ),
          localVariableTypes( _localVariablesTypes ) {}


    // Adds local variable. Returns false on conflict
    bool AddLocalVariable( const std::string& name, CTypeIdentifier type );

    TAccessModifier AccessModifier() const { return accessModifier; }
    const std::string& MethodName() const { return methodName; }
    CTypeIdentifier ReturnType() const { return returnType; }

// Return NotFound in CTypeIdentifier if variable not found
    CTypeIdentifier GetLocalVariableType( const std::string& name ) const;
    CTypeIdentifier GetArgumentType( const std::string& name ) const;

private:
    TAccessModifier accessModifier;
    std::string methodName;
    CTypeIdentifier returnType;
    std::unordered_map<std::string, CTypeIdentifier> argumentTypes;
    std::unordered_map<std::string, CTypeIdentifier> localVariableTypes;
};
